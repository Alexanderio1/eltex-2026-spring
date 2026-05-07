#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <stdlib.h>

#define MIN_PRIORITY 0
#define MAX_PRIORITY 255

typedef enum TrafficType {
    VOICE = 0,
    VIDEO,
    WEB,
    OTHER
} TrafficType;

typedef struct Packet {
    unsigned int packet_ID;
    unsigned char priority;
    TrafficType traffic_type;
} Packet;

static Packet* createPacket(unsigned int packet_ID, unsigned int priority, TrafficType traffic_type) {
    if (priority > MAX_PRIORITY) {
        return NULL;
    }

    Packet* packet = malloc(sizeof(Packet));
    if (packet == NULL) {
        return NULL;
    }

    packet->packet_ID = packet_ID;
    packet->priority = (unsigned char)priority;
    packet->traffic_type = traffic_type;

    return packet;
}

static void removePacket(Packet* packet) {
    free(packet);
}

static const char* trafficTypeToString(TrafficType traffic_type) {
    switch (traffic_type) {
        case VOICE: return "VOICE";
        case VIDEO: return "VIDEO";
        case WEB:   return "WEB";
        case OTHER: return "OTHER";
        default:    return "UNKNOWN";
    }
}

static void printPacket(const char* message, const Packet* packet) {
    if (packet == NULL) {
        printf("%s: packet not found\n", message);
        return;
    }

    printf("%s: packet_ID=%u, priority=%u, type=%s\n",
           message,
           packet->packet_ID,
           (unsigned int)packet->priority,
           trafficTypeToString(packet->traffic_type));
}

#endif
