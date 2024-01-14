#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


node_t* pq;
int pqSize;

bool IsEmpty();

void InitializePQ();

void Enqueue(node_t node);

node_t Dequeue();

void Swap(int index1, int index2);

void BubbleUp(int index);

void BubbleDown(int index);

void PrintPQ();