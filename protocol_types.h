#pragma once
#include <Arduino.h>

#pragma pack(push, 1)

struct SensorData {
    char client[16];
    int id;
    float temp;
    float volt;
};

struct SyncData {
    uint32_t next_sleep_seconds;
};

#pragma pack(pop)

struct NodeRecord {
    String client;
    int id = 0;
    float temp = NAN;
    float volt = NAN;
    uint32_t lastSeenMs = 0;
    String mac;
};