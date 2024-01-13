#include "priority_queue.h"

bool IsEmpty() {
    return pqSize == 0;
}

void InitializePQ() {
    pq = malloc(sizeof(node_t) * 10000);
    pqSize = 0;
}

void Enqueue(node_t node) {
    pqSize++;

    pq[pqSize] = node;
    BubbleUp(pqSize);
}

node_t Dequeue() {
    node_t node = pq[0];
    pqSize--;
    pq[0] = pq[pqSize];

    return node;
}

void Swap(int index1, int index2) {
    node_t temp = pq[index1];
    pq[index1] = pq[index2];
    pq[index2] = temp;
}

void BubbleUp(int index) {
    if (index == 0) {
        return;
    }
    int parentIndex = (index - 1) / 2;
    if (pq[index].totalHeatLoss < pq[parentIndex].totalHeatLoss) {
        Swap(index, parentIndex);
        BubbleUp(parentIndex);
    }
}

void BubbleDown(int index) {
    int leftChildIndex = index * 2 + 1;
    int rightChildIndex = index * 2 + 2;
    if (leftChildIndex >= pqSize) {
        return;
    }
    int minIndex = leftChildIndex;
    if (rightChildIndex < pqSize && pq[rightChildIndex].totalHeatLoss < pq[leftChildIndex].totalHeatLoss) {
        minIndex = rightChildIndex;
    }
    if (pq[index].totalHeatLoss > pq[minIndex].totalHeatLoss) {
        Swap(index, minIndex);
        BubbleDown(minIndex);
    }
}

void PrintPQ() {
    for (int i = 0; i < pqSize; i++) {
        printf("%d ", pq[i].totalHeatLoss);
    }
    printf("\n");
}