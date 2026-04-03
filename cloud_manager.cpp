#include "cloud_manager.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

CloudManager::CloudManager(const String& serverUrl)
    : _serverUrl(serverUrl) {}

bool CloudManager::postSensorData(const SensorData& data) {
    JsonDocument doc;
    doc["client"] = data.client;
    doc["node"] = data.id;
    doc["temp"] = data.temp;
    doc["volt"] = data.volt;

    String payload;
    serializeJson(doc, payload);
    return postJsonPayload(payload);
}

bool CloudManager::postQueuedItem(const QueuedCloudItem& item) {
    JsonDocument doc;
    doc["client"] = item.client;
    doc["node"] = item.node;
    doc["temp"] = item.temp;
    doc["volt"] = item.volt;

    String payload;
    serializeJson(doc, payload);
    return postJsonPayload(payload);
}

bool CloudManager::postJsonPayload(const String& payload) {
    if (_serverUrl.isEmpty()) {
        _lastError = "server_url empty";
        return false;
    }

    if (WiFi.status() != WL_CONNECTED) {
        _lastError = "WiFi not connected";
        return false;
    }

    HTTPClient http;
    if (!http.begin(_serverUrl)) {
        _lastError = "HTTP begin failed";
        return false;
    }

    http.setConnectTimeout(5000);
    http.setTimeout(5000);
    http.addHeader("Content-Type", "application/json");

    int code = http.POST(payload);
    if (code < 200 || code >= 300) {
        _lastError = "HTTP POST failed, code=" + String(code);
        http.end();
        return false;
    }

    http.end();
    _lastError = "";
    return true;
}

String CloudManager::getLastError() const {
    return _lastError;
}

void CloudManager::clearError() {
    _lastError = "";
}