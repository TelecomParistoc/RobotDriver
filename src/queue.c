#include "queue.h"
#include <stdlib.h>

struct queueElement {
    void* content;
    struct queueElement* nextElement;
}

static queueElement* queueHead = NULL;
static queueElement* queueTail = NULL;
static queueSize = 0;

void addToQueue(void* elementPtr) {
    struct queueElement* newElement = malloc(sizeof(struct queueElement));
    newElement->content = elementPtr;
    newElement->nextElement = NULL;
    if(queueTail != NULL)
        queueTail->nextElement = newElement;
    if(queueHead == NULL)
        queueHead = newElement;
    queueSize++;
}

void* getHead() {
    if(queueHead!=NULL)
        return queueHead->content;
    return NULL;
}
void removeHead() {
    if(queueHead != NULL) {
        free(queueHead->content);
        free(queueHead);
        queueHead = queueHead->nextElement;
        queueSize--;
    }
}

void clearQueue() {
    while(queueHead != NULL)
        removeHead();
}

int getQueueSize() { return queueSize; }
