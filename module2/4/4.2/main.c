#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "priority_queue.h"

#define RANDOM_MESSAGES_COUNT 10

static TrafficType randomTrafficType(void) {
    return (TrafficType)(rand() % 4);
}

static unsigned int randomPriority(void) {
    return rand() % (MAX_PRIORITY + 1);
}

static void addPacket(PriorityQueue* queue,
                      unsigned int packet_ID,
                      unsigned int priority,
                      TrafficType traffic_type) {
    Packet* packet = createPacket(packet_ID, priority, traffic_type);

    if (packet == NULL) {
        printf("Error: cannot create packet with priority %u\n", priority);
        clearPQ(queue);
        exit(EXIT_FAILURE);
    }

    if (!pushPQ(queue, packet)) {
        printf("Error: cannot add packet to queue\n");
        removePacket(packet);
        clearPQ(queue);
        exit(EXIT_FAILURE);
    }
}

static void generateMessages(PriorityQueue* queue) {
    for (unsigned int i = 1; i <= RANDOM_MESSAGES_COUNT; i++) {
        addPacket(queue, i, randomPriority(), randomTrafficType());
    }

    addPacket(queue, 1001, 255, VOICE);
    addPacket(queue, 1002, 200, VIDEO);
    addPacket(queue, 1003, 128, WEB);
    addPacket(queue, 1004, 0, OTHER);
}

static void processPacket(const char* action, Packet* packet) {
    printPacket(action, packet);
    removePacket(packet);
}

int main(void) {
    srand((unsigned int)time(NULL));

    PriorityQueue queue;
    initPQ(&queue);

    printf("Generating messages...\n");
    generateMessages(&queue);

    printf("\nInitial queue:\n");
    printPQ(&queue);

    printf("\n1) Extract first packet:\n");
    processPacket("popFirstPQ", popFirstPQ(&queue));

    printf("\n2) Extract packet with priority 128:\n");
    processPacket("popByPriorityPQ(128)", popByPriorityPQ(&queue, 128));

    printf("\n3) Extract packet with priority not lower than 200:\n");
    processPacket("popPriorityNotLowerPQ(200)", popPriorityNotLowerPQ(&queue, 200));

    printf("\n4) Try to extract packet with invalid priority 300:\n");
    processPacket("popByPriorityPQ(300)", popByPriorityPQ(&queue, 300));

    printf("\nQueue after extractions:\n");
    printPQ(&queue);

    clearPQ(&queue);

    return 0;
}
