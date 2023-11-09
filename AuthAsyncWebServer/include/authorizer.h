#ifndef AUTHORIZER_H
#define AUTHORIZER_H

#include "ESPAsyncWebServer.h"

class Authorizer
{
public:
    static bool isAuthorized(AsyncWebServerRequest *request)
    {
        if(!request->authenticate("admin", "password")){
            return false;
        }

        return true;
    }

    static bool authorizationFilter(AsyncWebServerRequest *request)
    {
        if (!request->authenticate("admin", "password"))
        {
            return false;
        }

        return true;
    }
};

#endif