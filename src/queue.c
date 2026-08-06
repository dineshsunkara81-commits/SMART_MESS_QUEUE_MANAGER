#include "../include/queue.h"

void initializeQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int isEmpty(Queue *q) {
    return q->count == 0;
}

int isFull(Queue *q) {
    return q->count == MAX_SIZE;
}

int enqueue(Queue *q, Token token) {
    if (isFull(q)) {
        return 0;
    }
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->items[q->rear] = token;
    q->count++;
    return 1;
}

Token dequeue(Queue *q) {
    Token emptyToken;
    emptyToken.tokenNumber = -1;
    strcpy(emptyToken.studentName, "");
    emptyToken.status = -1;
    
    if (isEmpty(q)) {
        return emptyToken;
    }
    
    Token token = q->items[q->front];
    q->front = (q->front + 1) % MAX_SIZE;
    q->count--;
    return token;
}

Token peek(Queue *q) {
    Token emptyToken;
    emptyToken.tokenNumber = -1;
    strcpy(emptyToken.studentName, "");
    emptyToken.status = -1;
    
    if (isEmpty(q)) {
        return emptyToken;
    }
    return q->items[q->front];
}

void displayQueue(Queue *q) {
    if (isEmpty(q)) {
        printf("\n+-------------------------------------+\n");
        printf("|        QUEUE IS EMPTY!             |\n");
        printf("+-------------------------------------+\n");
        return;
    }
    
    printf("\n+-----------------------------------------------------------+\n");
    printf("|                   CURRENT QUEUE STATUS                    |\n");
    printf("+-------------+-------------+-----------------------------+\n");
    printf("|   Position  |  Token No.  |     Student Name             |\n");
    printf("+-------------+-------------+-----------------------------+\n");
    
    int index = q->front;
    int position = 1;
    for (int i = 0; i < q->count; i++) {
        printf("|     %-7d |    %-8d |     %-25s|\n", 
               position, 
               q->items[index].tokenNumber,
               q->items[index].studentName);
        index = (index + 1) % MAX_SIZE;
        position++;
    }
    printf("+-------------+-------------+-----------------------------+\n");
    printf("Total: %d students waiting in queue\n", q->count);
}

int getQueueLength(Queue *q) {
    return q->count;
}

int getTokenPosition(Queue *q, int tokenNumber) {
    if (isEmpty(q)) {
        return -1;
    }
    
    int index = q->front;
    int position = 1;
    for (int i = 0; i < q->count; i++) {
        if (q->items[index].tokenNumber == tokenNumber) {
            return position;
        }
        index = (index + 1) % MAX_SIZE;
        position++;
    }
    return -1;
}