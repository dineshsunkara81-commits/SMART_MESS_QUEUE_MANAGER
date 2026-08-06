#include <stdio.h>
#include "queue.h"
#include "student.h"
#include "admin.h"

int main()
{
    Queue q;
    int choice;

    // Initialize Queue
    initializeQueue(&q);

    do
    {
        printf("\n=========================================\n");
        printf("     SMART MESS QUEUE MANAGER\n");
        printf("=========================================\n");
        printf("1. Student\n");
        printf("2. Admin\n");
        printf("3. Exit\n");
        printf("-----------------------------------------\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                studentMenu(&q);
                break;

            case 2:
                adminMenu(&q);
                break;

            case 3:
                printf("\nThank You for Using Smart Mess Queue Manager.\n");
                break;

            default:
                printf("\nInvalid Choice! Please Try Again.\n");
        }

    } while(choice != 3);

    return 0;
}