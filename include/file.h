#ifndef FILE_H
#define FILE_H

#include "queue.h"

void loadQueueFromFile(Queue *q, int *nextTokenNumber);
void saveQueueToFile(Queue *q);
void saveToHistory(Token token);
void generateDailyReport(Queue *q);
void saveStudentRecord(Token token);
void loadTokenHistory();

#endif