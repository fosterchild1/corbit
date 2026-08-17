#include <stdlib.h>
#include <string.h>
#include "Utils/array.h"

Array Array_New(int elementSize) {
    return (Array){.data = NULL, .len = 0, .capacity = 0, .elementSize = elementSize};
}

// TODO: insert at index
void Array_Insert(Array* array, void* element) {
    int currSize = array->capacity;
    int neededSize = array->len + 1;
    
    // resize if needed
    if (neededSize > currSize) {
        int newSize = currSize * 2;
        if (currSize == 0) newSize = 1;

        void* temp = realloc(array->data, array->elementSize * newSize);
        if (temp == NULL) exit(EXIT_FAILURE);
        
        array->data = temp;
        array->capacity = newSize;
    }

    array->len++;
    void* elementIdx = (char*)array->data + array->elementSize * (neededSize - 1);
    memcpy(elementIdx, element, array->elementSize);
}

void* Array_Remove(Array* array, int index) {
    int elemSize = array->elementSize;
    //void* element = (char*)array->data + (index * elemSize);

    int newSize = --array->len;

    // shift all elements after index backwards by 1
    char* dst = (char*)array->data + index * elemSize;
    memmove(dst, dst + elemSize, (newSize - index) * elemSize);
  
    // TODO: return element
    if (newSize == 0) {
        Array_Free(array);
        return NULL;
    }

    void* newData = realloc(array->data, array->elementSize * newSize);
    if (newData == NULL) exit(EXIT_FAILURE);
    
    array->data = newData;
    array->capacity = newSize;

    return NULL;
}

void Array_Free(Array* array) {
    free(array->data); array->data = NULL;
    array->capacity = 0; array->len = 0;
}
