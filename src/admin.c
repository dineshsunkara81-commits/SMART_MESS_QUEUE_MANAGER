#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/admin.h"
#include "../include/file.h"

#define ADMIN_USER "admin"
#define ADMIN_PASS "admin123"

int adminLogin() {
    char username[20], password[20];
    printf("\n+-------------------------------------+\n");
    printf("|             ADMIN LOGIN              |\n");
    printf("+-------------------------------------+\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    
    if (strcmp(username, ADMIN_USER) == 0 && 
        strcmp(password, ADMIN_PASS) == 0) {
        printf("\nLogin successful!\n");
        return 1;
    } else {
        printf("\nInvalid credentials!\n");
        return 0;
    }
}

void adminViewQueue(Queue *q) {
    displayQueue(q);
}

void adminCallNext(Queue *q) {
    if (isEmpty(q)) {
        printf("\nQueue is empty! No tokens to call.\n");
        return;
    }
    
    Token nextToken = dequeue(q);
    if (nextToken.tokenNumber != -1) {
        saveQueueToFile(q);
        saveToHistory(nextToken);
        
        printf("\n+-------------------------------------+\n");
        printf("|         CALLING NEXT TOKEN          |\n");
        printf("+-------------------------------------+\n");
        printf("| Token Number: %d                    |\n", nextToken.tokenNumber);
        printf("| Student Name: %s                    |\n", nextToken.studentName);
        printf("+-------------------------------------+\n");
        
        // Send SMS Alert
        printf("\n---------------------------------\n");
        printf("SMS ALERT\n");
        printf("Your Token Number : %d\n", nextToken.tokenNumber);
        printf("Please come to the mess counter.\n");
        printf("---------------------------------\n");
    }
}

void adminViewReports(Queue *q) {
    printf("\n+-------------------------------------+\n");
    printf("|           DAILY REPORTS              |\n");
    printf("+-------------------------------------+\n");
    
    printf("\nCURRENT QUEUE STATUS:\n");
    printf("  Total Waiting: %d\n", getQueueLength(q));
    
    FILE *history = fopen("../data/history.txt", "r");
    if (history != NULL) {
        int count = 0;
        int totalTime = 0;
        int tokenNum;
        char name[50];
        time_t start, end;
        
        while (fscanf(history, "%d,%[^,],%ld,%ld\n", 
                      &tokenNum, name, &start, &end) == 4) {
            int servingTime = (int)(end - start) / 60;
            if (servingTime < 1) servingTime = 1;
            totalTime += servingTime;
            count++;
        }
        fclose(history);
        
        printf("\nTODAY'S STATISTICS:\n");
        printf("  Total Served: %d\n", count);
        if (count > 0) {
            printf("  Average Serving Time: %d minutes\n", totalTime / count);
        }
    } else {
        printf("\nNo history data available.\n");
    }
    
    generateDailyReport(q);
    printf("\nReport generated and saved!\n");
}

void adminMenu(Queue *q) {
    if (!adminLogin()) {
        return;
    }
    
    int choice;
    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. View Queue\n");
        printf("2. Call Next Token\n");
        printf("3. View Reports\n");
        printf("4. Back\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                adminViewQueue(q);
                break;
            case 2:
                adminCallNext(q);
                break;
            case 3:
                adminViewReports(q);
                break;
            case 4:
                printf("\nReturning to Main Menu...\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while(choice != 4);
}