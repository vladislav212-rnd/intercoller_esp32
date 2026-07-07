#include <Arduino.h>

#include "WebManager.h"

WebManager web;

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("================================");
    Serial.println(" ESP32 OBD Dashboard");
    Serial.println("================================");

    web.begin();

    Serial.println("System started.");
}

void loop()
{
    web.loop();
}
