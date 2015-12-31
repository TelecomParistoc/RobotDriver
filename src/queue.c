#include "queue.h"
#include <stdlib.h>

struct queueElement {
    void* content;
    struct queueElement* nextElement;
}

static queueElement* queueHead = NULL;
static queueElement* queueTail = NULL;

void addToQueue(void* elementPtr) {
    struct queueElement* newElement = malloc(sizeof(struct queueElement));
    newElement->content = elementPtr;
    newElement->nextElement = NULL;
    if(queueTail != NULL)
        queueTail->nextElement = newElement;
    if(queueHead == NULL)
        queueHead = newElement;
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
    }
}

void clearQueue() {
    while(queueHead != NULL)
        removeHead();
}

int isQueueEmpty() { return queueHead == NULL; }
