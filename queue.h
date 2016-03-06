#ifndef _QUEUE_H
#define _QUEUE_H_

#include "structures.h"

void pushQueueValue(queue **head, huffman value);
void pushQueueNode(queue **head, queue *new);
queue *popQueue(queue **head);
queue *createQueue(huffman *Huffman, int n);


#endif