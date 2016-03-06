#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "huffman.h"

/* Create a Huffman Tree */
queue *createHuffmanTree(queue *head){
	queue *aux1, *aux2;
	while(head->next){
		/* Pop two elements out of the queue */
		aux1 = popQueue(&head);
		aux2 = popQueue(&head);
		queue *combined = (queue*)malloc(sizeof(queue));
		/* Create combined node */
		if(aux1->value.frequency <= aux2->value.frequency){
			combined->left = aux1;
			combined->right = aux2;
		}
		else{
			combined->left = aux2;
			combined->right = aux1;
		}
		/* New node frequency is the sum of the combined nodes */
		combined->value.frequency = aux1->value.frequency + aux2->value.frequency;
		/* Push back into queue */
		pushQueueNode(&head, combined);
	}
	return head;
}

queue *HuffmanTree(FILE *fin, uint32_t *numberOfChars){
	unsigned char c;
	int v[256] = {0};
	int i;
	struct huffman Huffman[256];
	/* Read all characters from file */
	while(fscanf(fin, "%c", &c) && !feof(fin)){
		v[c]++;
		(*numberOfChars)++;
	}
	/* Exception
	 * NULL counted more than once */
	if(v[0] > 0)
		v[0]--;
	/* Create node for each character */
	int countNodes = 0;
	for(i = 0; i < 256; i++){
		if(v[i] != 0){
			Huffman[countNodes].caracter = i;
			Huffman[countNodes].frequency = (float)(v[i])/(float)(*numberOfChars);
			countNodes++;
		}
	}
	/* Priority queue */
	queue *head = createQueue(Huffman, countNodes);
	/* Create HuffmanTree */
	queue *root = createHuffmanTree(head);
	return root;
}

/* Number of tree nodes */
uint16_t countNodes(queue *root){
	uint16_t count = 0;
	if(root)
		count = 1 + countNodes(root->left) + countNodes(root->right);
	return count;
}

/* Free tree */
void freeTree(queue *root){
	if(root == NULL)
		return;
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}

/* Huffman Codes */
HuffmanCode huffCode[1024];
int huffCodesCount = 0; // total number of codes
void getCodes(queue *root, int length){
	/* Leaf node -> obtained code */
	if(root->left == NULL && root->right == NULL){
		huffCode[huffCodesCount].length = length;
		huffCode[huffCodesCount].caracter = root->value.caracter;
		huffCodesCount++;
		int i;
		/* Copy previous path in the new code */
		for(i = 0; i < huffCode[huffCodesCount - 1].length; i++)
			huffCode[huffCodesCount].code[i] = huffCode[huffCodesCount - 1].code[i];
	}
	/* Generate code left */
	if(root->left){
		huffCode[huffCodesCount].code[length] = 0;
		getCodes(root->left, length + 1);
	}
	/* Generate code right */
	if(root->right){
		huffCode[huffCodesCount].code[length] = 1;
		getCodes(root->right, length + 1);
	}
}


/* TagHuffmanNode */
/* Tree indexing - preorder */
int treeIndex = 0; // current index of the current node in tree;
void treeIndexing(queue *root){
	if(root == NULL)
		return;
	root->indiceVec = treeIndex;
	treeIndex++;
	treeIndexing(root->left);
	treeIndexing(root->right);
}

/* Create TagHuffmanNode array */
int indexTagHuffman = 0; //current index of a TagHuffmanNode element;
TagHuffmanNode vectorTagHuffman[4096];
void createTagHuffmanVector(queue *root){
	/* Non-leafe node */
	if(root->right != NULL && root->left != NULL){
		vectorTagHuffman[indexTagHuffman].left = root->left->indiceVec;
		vectorTagHuffman[indexTagHuffman].right = root->right->indiceVec;
		indexTagHuffman++;
	}
	/* Leaf node */
	if(root->left == NULL && root->right == NULL){
		vectorTagHuffman[indexTagHuffman].value = root->value.caracter;
		vectorTagHuffman[indexTagHuffman].left = -1;
		vectorTagHuffman[indexTagHuffman].right = -1;
		indexTagHuffman++;
	}
	if(root->left)
		createTagHuffmanVector(root->left);
	if(root->right)
		createTagHuffmanVector(root->right);
}

/* Compress file */
void compressFile(FILE *fin, FILE *fout){
	 	/* Create Huffman tree - indexed */
		uint32_t numberOfChars = 0;
		queue *Huffman = HuffmanTree(fin, &numberOfChars);
		uint16_t numberOfNodes = countNodes(Huffman);
		treeIndexing(Huffman);
		/* Create TagHuffmanNode array */
		createTagHuffmanVector(Huffman);
		/* Get codes for each character */
		getCodes(Huffman, 0);
		/* Write in binary file - number of characters and number of nodes */
		fwrite(&numberOfChars, sizeof(uint32_t), 1, fout);
		fwrite(&numberOfNodes, sizeof(uint16_t), 1, fout);
		/* Prase file */
		unsigned char c;
		rewind(fin);
		int i, j;
		/* Write TagHuffmanNode array */
		for(i = 0; i < indexTagHuffman; i++)
			fwrite(&vectorTagHuffman[i], sizeof(TagHuffmanNode), 1, fout);
		/* Parse file and binary compression */
		char code = 0;
		int bitCount = 0; // number of bits
		while(fscanf(fin, "%c", &c) && !feof(fin)){
			/* Search for character code */
			for(i = 0; i < huffCodesCount; i++){
				if (huffCode[i].caracter == c){
					/* Write in file according to the code */
					for(j = 0; j < huffCode[i].length; j++){
						if(huffCode[i].code[j] == 0)
							code = code << 1;
						else
							code = (code << 1) | 1;
						bitCount++;
						/* Write in file when 8 bits are counted */
						if(bitCount == 8){
							fwrite(&code, sizeof(char), 1, fout);
							bitCount = 0;
						}
					}
					break;
				}
			}
		}
		/* Bits left */
		if(bitCount != 0){
			while(bitCount != 8){
				code = code << 1;
				bitCount++;
			}
			fwrite(&code, sizeof(char), 1, fout);
		}
		/* Eliberare arbore */
		freeTree(Huffman);
}
/* Decompress file */
/* Transform char in bits */
void charToBinary(char c, int *bits){
	int i;
	for(i = 0; i <= 7; i++){
		if((c & (1 << i)) == 0)
			bits[7 - i] = 0; // bit 0
		else
			bits[7 - i] = 1; // bit 1
	}
}
void decompressFile(FILE* fin, FILE* fout){
	 	/* Read number of characters from file 
	 	 * Read number of nodes of Huffman Tree */
	 	uint32_t numberOfChars, countChars = 0;
		uint16_t numberOfNodes;
		fread(&numberOfChars, sizeof(uint32_t), 1, fin);
		fread(&numberOfNodes, sizeof(uint16_t), 1, fin);
		/* Read Huffman Tree */
		TagHuffmanNode HuffmanVector[numberOfNodes];
		int i;
		for(i = 0; i < numberOfNodes; i++){
			fread(&HuffmanVector[i], sizeof(TagHuffmanNode), 1, fin);
		}
		/* Decompress */
		char buffer;
		int semafor = 0;
		int indice = 0;
		while(fread(&buffer, sizeof(char), 1, fin) && !feof(fin)){
			int bits[8];
			/* Get 8 bits from file */
			charToBinary(buffer, bits);
			/* Decode according to Huffman Tree */
			for(i = 0; i < 8; i++){
				/* Number of characters - finished */
				if(countChars == numberOfChars){
					semafor = 1;
					break;
				}
				/* Parse the tree */
				if(bits[i] == 1)
					indice = HuffmanVector[indice].right;
				else
					indice = HuffmanVector[indice].left;
				/* Leaf-node - a character */
				if(HuffmanVector[indice].right == -1 && HuffmanVector[indice].left == -1){
					fprintf(fout, "%c", HuffmanVector[indice].value);
					countChars++;
					indice = 0;
				}
			}
			/* If number of characters is finished */
			if(semafor == 1)
				break;
		}
}