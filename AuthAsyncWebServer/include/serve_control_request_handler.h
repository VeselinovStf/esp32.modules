#ifndef SERVE_CONTROL_REQUEST_HANDLER_H
#define SERVE_CONTROL_REQUEST_HANDLER_H

#include "ESPAsyncWebServer.h"

class ServeControlRequestHandler : public AsyncWebHandler
{
public:
    ServeControlRequestHandler() {}
    virtual ~ServeControlRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        if (request->method() == HTTP_GET && request->url() == "/control")
        {
            return true;
        }

        return false;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {

        ESP_LOGD("ServeControlRequestHandler", "[GetControl]:[INF]: [SERVE] /control");

        request->send(200, "text/html", "<h1>Control</h1>");
    }
};

#endif