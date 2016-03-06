#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

/* Character - frequency association */
typedef struct huffman{
	unsigned char caracter;
	float frequency;
}huffman;

/* Tree/queue node */
typedef struct queue{
	int indiceVec;
	huffman value;
	struct queue *next;
	struct queue *left;
	struct queue *right;
}queue;

/* Huffman Codes */
typedef struct HuffmanCode{
	unsigned char caracter;
	int length;
	int code[1024];
}HuffmanCode;

/* TagHuffmanNode */
typedef struct TagHuffmanNode{
	unsigned char value;
	int16_t left;
	int16_t right;
}__attribute__((__packed__)) TagHuffmanNode;

#endif