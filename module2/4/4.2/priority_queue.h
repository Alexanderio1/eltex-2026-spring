#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "packet.h"

typedef struct PQNode {
    Packet* packet;
    struct PQNode* next;
} PQNode;

typedef struct PriorityQueue {
    PQNode* head;
    size_t size;
} PriorityQueue;

static void initPQ(PriorityQueue* pq) {
    pq->head = NULL;
    pq->size = 0;
}

static int isEmptyPQ(const PriorityQueue* pq) {
    return pq == NULL || pq->head == NULL;
}

static PQNode* createNode(Packet* packet) {
    PQNode* node = malloc(sizeof(PQNode));
    if (node == NULL) {
        return NULL;
    }

    node->packet = packet;
    node->next = NULL;

    return node;
}

static Packet* detachNode(PriorityQueue* pq, PQNode* prev, PQNode* current) {
    Packet* packet = current->packet;

    if (prev == NULL) {
        pq->head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    pq->size--;

    return packet;
}


static int pushPQ(PriorityQueue* pq, Packet* packet) {
    if (pq == NULL || packet == NULL) {
        return 0;
    }

    PQNode* node = createNode(packet);
    if (node == NULL) {
        return 0;
    }

    if (pq->head == NULL || packet->priority > pq->head->packet->priority) {
        node->next = pq->head;
        pq->head = node;
        pq->size++;
        return 1;
    }

    PQNode* current = pq->head;

    while (current->next != NULL &&
           current->next->packet->priority >= packet->priority) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    pq->size++;

    return 1;
}

static Packet* popFirstPQ(PriorityQueue* pq) {
    if (isEmptyPQ(pq)) {
        return NULL;
    }

    return detachNode(pq, NULL, pq->head);
}

static Packet* popByPriorityPQ(PriorityQueue* pq, unsigned int priority) {
    if (isEmptyPQ(pq) || priority > MAX_PRIORITY) {
        return NULL;
    }

    PQNode* prev = NULL;
    PQNode* current = pq->head;

    while (current != NULL) {
        if (current->packet->priority == priority) {
            return detachNode(pq, prev, current);
        }

        if (current->packet->priority < priority) {
            break;
        }

        prev = current;
        current = current->next;
    }

    return NULL;
}

static Packet* popPriorityNotLowerPQ(PriorityQueue* pq, unsigned int min_priority) {
    if (isEmptyPQ(pq) || min_priority > MAX_PRIORITY) {
        return NULL;
    }

    if (pq->head->packet->priority >= min_priority) {
        return detachNode(pq, NULL, pq->head);
    }

    return NULL;
}

static void printPQ(const PriorityQueue* pq) {
    if (isEmptyPQ(pq)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue size: %zu\n", pq->size);

    for (PQNode* current = pq->head; current != NULL; current = current->next) {
        printf("packet_ID=%u, priority=%u, type=%s\n",
               current->packet->packet_ID,
               (unsigned int)current->packet->priority,
               trafficTypeToString(current->packet->traffic_type));
    }
}

static void clearPQ(PriorityQueue* pq) {
    if (pq == NULL) {
        return;
    }

    PQNode* current = pq->head;

    while (current != NULL) {
        PQNode* next = current->next;
        removePacket(current->packet);
        free(current);
        current = next;
    }

    pq->head = NULL;
    pq->size = 0;
}

#endif
