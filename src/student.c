#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/student.h"
#include "../include/file.h"
#include "../include/ai.h"

void sendSMSAlert(int tokenNumber, char *message) {
    printf("\n---------------------------------\n");
    printf("SMS ALERT\n");
    printf("Your Token Number : %d\n", tokenNumber);
    printf("%s\n", message);
    printf("---------------------------------\n");
}

void generateToken(Queue *q, int *nextTokenNumber) {
    char name[MAX_NAME_LEN];
    printf("Enter Student Name: ");
    getchar();
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';
    
    if (strlen(name) == 0) {
        printf("Error: Name cannot be empty!\n");
        return;
    }
    
    if (isFull(q)) {
        printf("Error: Queue is full! Please try later.\n");
        return;
    }
    
    Token newToken;
    newToken.tokenNumber = (*nextTokenNumber)++;
    strcpy(newToken.studentName, name);
    newToken.timestamp = time(NULL);
    newToken.status = 0;
    
    if (enqueue(q, newToken)) {
        saveQueueToFile(q);
        saveStudentRecord(newToken);
        printf("\nToken Added Successfully.\n");
        sendSMSAlert(newToken.tokenNumber, "Please come to the mess.");
    } else {
        printf("Error: Could not generate token!\n");
    }
}

void viewQueuePosition(Queue *q) {
    if (isEmpty(q)) {
        printf("\nQueue is empty!\n");
        return;
    }
    
    int tokenNumber;
    printf("Enter Token Number: ");
    scanf("%d", &tokenNumber);
    
    int position = getTokenPosition(q, tokenNumber);
    if (position == -1) {
        printf("Token not found in queue!\n");
    } else {
        printf("Your position in queue: %d\n", position);
        printf("Total people ahead: %d\n", position - 1);
    }
}

void viewWaitingTime(Queue *q) {
    if (isEmpty(q)) {
        printf("\nQueue is empty! No waiting time to display.\n");
        return;
    }
    
    int tokenNumber;
    printf("Enter Token Number: ");
    scanf("%d", &tokenNumber);
    
    int position = getTokenPosition(q, tokenNumber);
    if (position == -1) {
        printf("Token not found in queue!\n");
        return;
    }
    
    predictWaitingTime(position);
}

void cancelToken(Queue *q) {
    if (isEmpty(q)) {
        printf("\nQueue is empty! No tokens to cancel.\n");
        return;
    }
    
    int tokenNumber;
    printf("Enter Token Number to Cancel: ");
    scanf("%d", &tokenNumber);
    
    int position = getTokenPosition(q, tokenNumber);
    if (position == -1) {
        printf("Token not found in queue!\n");
        return;
    }
    
    Queue tempQueue;
    initializeQueue(&tempQueue);
    
    int index = q->front;
    for (int i = 0; i < q->count; i++) {
        if (q->items[index].tokenNumber != tokenNumber) {
            enqueue(&tempQueue, q->items[index]);
        }
        index = (index + 1) % MAX_SIZE;
    }
    
    *q = tempQueue;
    saveQueueToFile(q);
    
    printf("\nToken %d cancelled successfully!\n", tokenNumber);
}

void studentMenu(Queue *q, int *nextTokenNumber) {
    int choice;
    do {
        printf("\n===== STUDENT MENU =====\n");
        printf("1. Generate Token\n");
        printf("2. View Queue Position\n");
        printf("3. View Waiting Time\n");
        printf("4. Cancel Token\n");
        printf("5. View Token History\n");
        printf("6. Back\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                generateToken(q, nextTokenNumber);
                break;
            case 2:
                viewQueuePosition(q);
                break;
            case 3:
                viewWaitingTime(q);
                break;
            case 4:
                cancelToken(q);
                break;
            case 5:
                loadTokenHistory();
                break;
            case 6:
                printf("\nReturning to Main Menu...\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while(choice != 6);
}