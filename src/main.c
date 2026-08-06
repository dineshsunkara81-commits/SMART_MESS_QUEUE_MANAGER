#include <stdio.h>
#include <stdlib.h>
#include "../include/queue.h"
#include "../include/student.h"
#include "../include/admin.h"
#include "../include/file.h"

int main() {
    Queue queue;
    int nextTokenNumber = 1;
    int choice;
    
    initializeQueue(&queue);
    loadQueueFromFile(&queue, &nextTokenNumber);
    
    printf("\n=========================================\n");
    printf("     SMART MESS QUEUE MANAGER\n");
    printf("=========================================\n");
    
    while(1) {
        printf("\n1. Student\n");
        printf("2. Admin\n");
        printf("3. Exit\n");
        printf("-----------------------------------------\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                studentMenu(&queue, &nextTokenNumber);
                break;
            case 2:
                adminMenu(&queue);
                break;
            case 3:
                printf("\n=========================================\n");
                printf("     Thank You for Using the System!\n");
                printf("=========================================\n");
                saveQueueToFile(&queue);
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    
    return 0;
}