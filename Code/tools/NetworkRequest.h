#pragma once
#include "ceblanktc1.h"


// #include <../../CryPlugins/CryHTTP/Interface/IHttpPlugin.h> // from Code/CryPlugins/CryHTTP/Interface/IHttpPlugin.h
// #include <../../CryPlugins/CryHTTP/Module/HttpRequestImpl_cURL.h> // from Code/CryPlugins/CryHTTP/Module/HttpRequestImpl_cURL.h


#include <cpr/cpr.h>


namespace ceblanktc1
{
namespace tools
{


/**
 * todo: check how CryHttp works
 * for GET/POST/PUT/DELETE & etc
 * 
 * but not websocket.
 * 
 */
class NetworkRequest
{
public:
    /* data */

public:
    NetworkRequest(/* args */);
    virtual ~NetworkRequest();

public:
    static std::string TBD2()
    {
        std::string url = "https://raw.githubusercontent.com/prothegee/prothegee/main/data/users.json";

        cpr::Response r = cpr::Get(cpr::Url{url});

        return r.text;
    }
};



} // namespace tools
} // namespace ceblanktc1
