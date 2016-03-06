#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "queue.h"

/* Insert a huffman value */
void pushQueueValue(queue **head, huffman value){
	queue *new = (queue*)malloc(sizeof(queue));
	struct queue *temp = *head;
	new->value = value;
	new->next = NULL;
	if(temp == NULL)
		*head = new;
	else{
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
}

/* Insert a huffman value in the coressponding position */
void pushQueueNode(queue **head, queue *new){
	struct queue *temp = *head;
	if(temp == NULL)
		*head = new;
	else{
		while(temp->next != NULL && new->value.frequency > temp->value.frequency){
			temp = temp->next;
		}
		new->next = temp->next;
		temp->next = new;
	}
}

/* Pop element from queue */
queue *popQueue(queue **head){
	struct queue *temp = *head;
	if(temp == NULL)
		return NULL;
	else if(temp->next == NULL){
		*head = NULL;
		return temp;
	} else {
		temp = *head;
		*head = (*head)->next;
		return temp;
	}
}

/* Create priority queue */
queue *createQueue(huffman *Huffman, int n){
	queue *head = NULL;
	int i, j;
	huffman aux;
	/* Order after frequency */
	for(i = 0; i < n; i++)
		for(j = i; j < n; j++)
			if(Huffman[i].frequency > Huffman[j].frequency){
				aux = Huffman[i];
				Huffman[i] = Huffman[j];
				Huffman[j] = aux;
			}
	/* Insert each element in the queue */
	for(i = 0; i < n; i++)
		pushQueueValue(&head, Huffman[i]);
	return head;
}