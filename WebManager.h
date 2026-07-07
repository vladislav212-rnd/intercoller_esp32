#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

class WebManager
{
public:

    WebManager();

    bool begin();

    void loop();

    void broadcastData(
        uint16_t rpm,
        int8_t intakeTemp,
        float throttle);

private:

    AsyncWebServer server;
    AsyncWebSocket websocket;

    void startWiFi();

    bool startFilesystem();

    void setupRoutes();

    void setupWebSocket();

    void handleWebSocketEvent(
        AsyncWebSocket *server,
        AsyncWebSocketClient *client,
        AwsEventType type,
        void *arg,
        uint8_t *data,
        size_t len);
};

#endif
