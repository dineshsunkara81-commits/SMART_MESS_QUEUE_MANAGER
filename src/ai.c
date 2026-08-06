#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/ai.h"

void predictWaitingTime(int position) {
    FILE *file = fopen("../data/history.txt", "r");
    if (file == NULL) {
        printf("\n+-----------------------------------------------------+\n");
        printf("| No historical data available. Using default values. |\n");
        printf("+-----------------------------------------------------+\n");
        printf("\nEstimated Waiting Time: %d minutes\n", position * 5);
        printf("Crowd Level: ");
        if (position <= 5) printf("LOW\n");
        else if (position <= 15) printf("MEDIUM\n");
        else if (position <= 30) printf("HIGH\n");
        else printf("VERY HIGH\n");
        return;
    }
    
    int totalTime = 0;
    int count = 0;
    int tokenNum;
    char name[50];
    time_t start, end;
    
    while (fscanf(file, "%d,%[^,],%ld,%ld\n", 
                  &tokenNum, name, &start, &end) == 4) {
        int servingTime = (int)(end - start) / 60;
        if (servingTime > 0 && servingTime < 60) {
            totalTime += servingTime;
            count++;
        }
    }
    fclose(file);
    
    printf("\n+-----------------------------------------------------+\n");
    printf("|           AI WAITING TIME PREDICTION               |\n");
    printf("+-----------------------------------------------------+\n");
    
    int estimatedTime;
    char crowdLevel[20];
    
    if (count > 0) {
        int avgTime = totalTime / count;
        estimatedTime = avgTime * position;
        if (estimatedTime < 1) estimatedTime = 1;
        if (estimatedTime > 120) estimatedTime = 120;
    } else {
        estimatedTime = position * 5;
        if (estimatedTime < 1) estimatedTime = 1;
        if (estimatedTime > 120) estimatedTime = 120;
    }
    
    if (position <= 5) {
        strcpy(crowdLevel, "LOW");
    } else if (position <= 15) {
        strcpy(crowdLevel, "MEDIUM");
    } else if (position <= 30) {
        strcpy(crowdLevel, "HIGH");
    } else {
        strcpy(crowdLevel, "VERY HIGH");
    }
    
    printf("| Your Position in Queue: %d                         |\n", position);
    printf("| Estimated Waiting Time: %d minutes                 |\n", estimatedTime);
    printf("| Current Crowd Level: %s                            |\n", crowdLevel);
    printf("+-----------------------------------------------------+\n");
}