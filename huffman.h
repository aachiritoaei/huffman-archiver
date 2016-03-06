#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "queue.h"
#include "structures.h"

queue *createHuffmanTree(queue *head);
queue *HuffmanTree(FILE *fin, uint32_t *numberOfChars);
uint16_t countNodes(queue *root);
void freeTree(queue *root);
void getCodes(queue *root, int length);
void treeIndexing(queue *root);
void createTagHuffmanVector(queue *root);

void compressFile(FILE *fin, FILE *fout);
void charToBinary(char c, int *bits);
void decompressFile(FILE* fin, FILE* fout);

#endif