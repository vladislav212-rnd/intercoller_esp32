#include <Arduino.h>

#include "Config.h"
#include "WebManager.h"

WebManager web;

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("==================================");
    Serial.println(PROJECT_NAME);
    Serial.println(PROJECT_VERSION);
    Serial.println("==================================");

    if (!web.begin())
    {
        Serial.println("WebManager start failed!");

        while (true)
        {
            delay(1000);
        }
    }

    Serial.println("System ready.");
}

void loop()
{
    // Пока пусто.
    // AsyncWebServer не требует обработки в loop().
}
