// Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// IndexQueue structure
typedef struct {
    int currSize;
    Node* front;
    Node* rear;
} IndexQueue;

IndexQueue* createQueue();
int isEmpty(IndexQueue* q);
void enqueue(IndexQueue* q, int data);
int dequeue(IndexQueue* q);
int getFront(IndexQueue* q);
int size(IndexQueue* q);
