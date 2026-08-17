#pragma once

typedef struct {
    void* data;
    int len;
    int capacity;
    int elementSize;
} Array;

Array Array_New(int elementSize);

void Array_Insert(Array* array, void* element);
void* Array_Remove(Array* array, int index);
void Array_Free(Array* array);
