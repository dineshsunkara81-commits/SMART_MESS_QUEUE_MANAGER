#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100
#define MAX_NAME_LEN 50

typedef struct {
    int tokenNumber;
    char studentName[MAX_NAME_LEN];
    time_t timestamp;
    int status;
} Token;

typedef struct {
    Token items[MAX_SIZE];
    int front;
    int rear;
    int count;
} Queue;

void initializeQueue(Queue *q);
int isEmpty(Queue *q);
int isFull(Queue *q);
int enqueue(Queue *q, Token token);
Token dequeue(Queue *q);
Token peek(Queue *q);
void displayQueue(Queue *q);
int getQueueLength(Queue *q);
int getTokenPosition(Queue *q, int tokenNumber);

#endif