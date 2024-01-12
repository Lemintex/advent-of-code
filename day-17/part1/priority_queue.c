#include "priority_queue.h"

extern node_t* queueOfSeenNodes;
extern int queueOfSeenNodesSize;

bool IsSeenNode(node_t* queueOfSeenNodes, int queueOfSeenNodesSize, node_t* node)
{
    for (int i = 0; i < queueOfSeenNodesSize; i++)
    {
        if (queueOfSeenNodes[i].mapX == node->mapX && queueOfSeenNodes[i].mapY == node->mapY && queueOfSeenNodes[i].totalHeatLoss == node->totalHeatLoss)
        {
            return true;
        }
    }
    return false;
}

void swap(node_t* a, node_t* b)
{
    node_t temp = *a;
    *a = *b;
    *b = temp;
}

// Function to heapify the tree
void Heapify(node_t* arr, int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
 
    // If left child is larger than root
    if (l < n && arr[l].totalHeatLoss > arr[largest].totalHeatLoss)
        largest = l;
 
    // If right child is larger than largest so far
    if (r < n && arr[r].totalHeatLoss > arr[largest].totalHeatLoss)
        largest = r;
 
    // If largest is not root
    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);
 
        // Recursively heapify the affected sub-tree
        Heapify(arr, n, largest);
    }
}

// Function to insert an element into the tree
void InsertToPQ(node_t* arr, int* n, node_t* key)
{
    // Increase the size of the heap
    *n = *n + 1;
 
    // Insert the element at end of the tree
    int i = *n - 1;
    arr[i] = *key;
 
    // Fix the tree if it's unbalanced
    while (i != 0 && arr[(i - 1) / 2].totalHeatLoss < arr[i].totalHeatLoss)
    {
       swap(&arr[i], &arr[(i - 1) / 2]);
       i = (i - 1) / 2;
    }
}

// Function to delete an element from the tree
void DeleteFromPQ(node_t* tree, int* n, int i)
{
    // Decrease the size of the heap
    *n = *n - 1;
 
    // Move the last element to the root
    tree[i] = tree[*n];
 
    // Heapify the tree
    Heapify(tree, *n, i);
}