#include <stdio.h>
#include <stdlib.h>

#include "index_queue.h"

// Create a new node
Node* newNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

// Create an empty queue
IndexQueue* createQueue() {
    IndexQueue* q = (IndexQueue*)malloc(sizeof(IndexQueue));
    q->front = q->rear = NULL;
    q->currSize = 0;
    return q;
}

// Check if the queue is empty
int isEmpty(IndexQueue* q) {
    return q->front == NULL;
}

// Enqueue operation
void enqueue(IndexQueue* q, int data) {
    Node* node = newNode(data);
    if (isEmpty(q)) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    
    q->currSize++;
}

// Dequeue operation
int dequeue(IndexQueue* q) {
    if (isEmpty(q)) {
        printf("Queue Underflow\n");
        return -1;
    }
    
    Node* temp = q->front;
    int removedData = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    
    q->currSize--;
    return removedData;
}

// Get front element
int getFront(IndexQueue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return -1;
    }
    return q->front->data;
}

// Get queue size
int size(IndexQueue* q) {
    return q->currSize;
}
