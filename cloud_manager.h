#pragma once

#include <Arduino.h>
#include "protocol_types.h"
#include "cloud_queue_manager.h"

class CloudManager {
public:
    explicit CloudManager(const String& serverUrl);

    bool postSensorData(const SensorData& data);
    bool postQueuedItem(const QueuedCloudItem& item);

    String getLastError() const;
    void clearError();

private:
    String _serverUrl;
    String _lastError;

    bool postJsonPayload(const String& payload);
};