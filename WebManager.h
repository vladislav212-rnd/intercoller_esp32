#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>
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
        float throttle
    );

private:

    WebServer server;
    WebSocketsServer websocket;

    void startWiFi();

    bool startFilesystem();

    void setupRoutes();

    void handleWebSocketEvent(
        uint8_t client,
        WStype_t type,
        uint8_t * payload,
        size_t length
    );

    String getContentType(String filename);

    bool handleFileRead(String path);
};

#endif
