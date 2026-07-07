#include "WebManager.h"
#include "Config.h"


WebManager::WebManager()
    :
    server(HTTP_PORT),
    websocket(WS_PORT)
{

}



bool WebManager::begin()
{
    Serial.println("Starting WebManager");


    if(!startFilesystem())
    {
        Serial.println("LittleFS failed");
        return false;
    }


    startWiFi();


    setupRoutes();


    websocket.begin();

    websocket.onEvent(
        [this](uint8_t client,
               WStype_t type,
               uint8_t *payload,
               size_t length)
        {
            handleWebSocketEvent(
                client,
                type,
                payload,
                length
            );
        }
    );


    server.begin();


    Serial.println("Web server started");

    return true;
}



void WebManager::loop()
{
    server.handleClient();

    websocket.loop();
}





void WebManager::startWiFi()
{

    WiFi.mode(WIFI_AP);


    WiFi.softAPConfig(
        AP_IP,
        AP_GATEWAY,
        AP_SUBNET
    );


    WiFi.softAP(
        AP_SSID,
        AP_PASS
    );


    Serial.println();

    Serial.println("WiFi AP started");

    Serial.print("SSID: ");
    Serial.println(AP_SSID);

    Serial.print("IP: ");
    Serial.println(
        WiFi.softAPIP()
    );

}





bool WebManager::startFilesystem()
{

    if(!LittleFS.begin(true))
    {
        return false;
    }


    Serial.println("LittleFS mounted");

    return true;
}






void WebManager::setupRoutes()
{

    server.on(
        "/",
        HTTP_GET,
        [this]()
        {
            handleFileRead("/index.html");
        }
    );



    server.onNotFound(
        [this]()
        {
            if(!handleFileRead(server.uri()))
            {
                server.send(
                    404,
                    "text/plain",
                    "File not found"
                );
            }
        }
    );

}






bool WebManager::handleFileRead(String path)
{


    if(path.endsWith("/"))
    {
        path += "index.html";
    }



    String contentType =
        getContentType(path);



    if(!LittleFS.exists(path))
    {
        return false;
    }



    File file =
        LittleFS.open(path,"r");


    server.streamFile(
        file,
        contentType
    );


    file.close();


    return true;

}






String WebManager::getContentType(String filename)
{

    if(filename.endsWith(".html"))
        return "text/html";

    if(filename.endsWith(".css"))
        return "text/css";

    if(filename.endsWith(".js"))
        return "application/javascript";

    if(filename.endsWith(".png"))
        return "image/png";

    if(filename.endsWith(".jpg"))
        return "image/jpeg";

    if(filename.endsWith(".svg"))
        return "image/svg+xml";


    return "text/plain";

}







void WebManager::handleWebSocketEvent(
    uint8_t client,
    WStype_t type,
    uint8_t * payload,
    size_t length
)
{

    switch(type)
    {

        case WStype_CONNECTED:

            Serial.print("WebSocket client connected: ");
            Serial.println(client);

        break;



        case WStype_DISCONNECTED:

            Serial.print("WebSocket client disconnected: ");
            Serial.println(client);

        break;



        case WStype_TEXT:

            Serial.println(
                (char*)payload
            );

        break;



        default:

        break;
    }

}







void WebManager::broadcastData(
    uint16_t rpm,
    int8_t intakeTemp,
    float throttle
)
{


    StaticJsonDocument<128> doc;


    doc["rpm"] =
        rpm;


    doc["iat"] =
        intakeTemp;


    doc["throttle"] =
        throttle;



    String output;


    serializeJson(
        doc,
        output
    );



    websocket.broadcastTXT(
        output
    );

}
