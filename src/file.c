#include "../include/file.h"

void loadQueueFromFile(Queue *q, int *nextTokenNumber) {
    FILE *file = fopen("../data/tokens.txt", "r");
    if (file == NULL) {
        file = fopen("../data/tokens.txt", "w");
        if (file != NULL) {
            fprintf(file, "0\n");
            fclose(file);
        }
        initializeQueue(q);
        *nextTokenNumber = 1;
        return;
    }
    
    int count;
    if (fscanf(file, "%d\n", &count) != 1) {
        fclose(file);
        file = fopen("../data/tokens.txt", "w");
        if (file != NULL) {
            fprintf(file, "0\n");
            fclose(file);
        }
        initializeQueue(q);
        *nextTokenNumber = 1;
        return;
    }
    
    initializeQueue(q);
    
    if (count > MAX_SIZE) {
        count = MAX_SIZE;
    }
    
    for (int i = 0; i < count; i++) {
        Token token;
        if (fscanf(file, "%d,%[^,],%ld,%d\n", 
                   &token.tokenNumber, 
                   token.studentName,
                   &token.timestamp,
                   &token.status) == 4) {
            enqueue(q, token);
            if (token.tokenNumber >= *nextTokenNumber) {
                *nextTokenNumber = token.tokenNumber + 1;
            }
        }
    }
    
    fclose(file);
}

void saveQueueToFile(Queue *q) {
    FILE *file = fopen("../data/tokens.txt", "w");
    if (file == NULL) {
        printf("Error: Cannot save queue data!\n");
        return;
    }
    
    fprintf(file, "%d\n", q->count);
    
    if (!isEmpty(q)) {
        int index = q->front;
        for (int i = 0; i < q->count; i++) {
            fprintf(file, "%d,%s,%ld,%d\n",
                   q->items[index].tokenNumber,
                   q->items[index].studentName,
                   q->items[index].timestamp,
                   q->items[index].status);
            index = (index + 1) % MAX_SIZE;
        }
    }
    
    fclose(file);
}

void saveToHistory(Token token) {
    FILE *file = fopen("../data/history.txt", "a");
    if (file == NULL) {
        printf("Error: Cannot save to history!\n");
        return;
    }
    
    time_t now = time(NULL);
    fprintf(file, "%d,%s,%ld,%ld\n", 
            token.tokenNumber,
            token.studentName,
            token.timestamp,
            now);
    
    fclose(file);
}

void generateDailyReport(Queue *q) {
    FILE *report = fopen("../data/reports.txt", "a");
    if (report == NULL) {
        printf("Error: Cannot generate report!\n");
        return;
    }
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char date[30];
    strftime(date, 30, "%Y-%m-%d", tm_info);
    
    fprintf(report, "\n========== DAILY REPORT ==========\n");
    fprintf(report, "Date: %s\n", date);
    fprintf(report, "Current Queue Length: %d\n", 
            getQueueLength(q));
    fprintf(report, "===================================\n");
    
    fclose(report);
}

void saveStudentRecord(Token token) {
    FILE *file = fopen("../data/students.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d,%s,%ld\n", 
                token.tokenNumber, 
                token.studentName,
                token.timestamp);
        fclose(file);
    }
}

void loadTokenHistory() {
    FILE *file = fopen("../data/history.txt", "r");
    if (file == NULL) {
        printf("\n+-------------------------------------+\n");
        printf("|        No History Found!            |\n");
        printf("+-------------------------------------+\n");
        return;
    }
    
    printf("\n+-----------------------------------------------------------+\n");
    printf("|                   TOKEN HISTORY                           |\n");
    printf("+-------------+---------------------+----------------------+\n");
    printf("|  Token No.  |     Student Name    |   Serving Time (min) |\n");
    printf("+-------------+---------------------+----------------------+\n");
    
    int tokenNum;
    char name[50];
    time_t start, end;
    int count = 0;
    
    while (fscanf(file, "%d,%[^,],%ld,%ld\n", 
                  &tokenNum, name, &start, &end) == 4) {
        int servingTime = (int)(end - start) / 60;
        if (servingTime < 1) servingTime = 1;
        printf("|    %-8d |     %-18s |     %-14d |\n", 
               tokenNum, name, servingTime);
        count++;
    }
    
    printf("+-------------+---------------------+----------------------+\n");
    printf("Total Records: %d\n", count);
    fclose(file);
}