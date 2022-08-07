#pragma once
#include "ceblanktc1.h"


namespace ceblanktc1
{
namespace tools
{


class NetworkRequest
{
public:
    /* data */

public:
    NetworkRequest(/* args */);
    virtual ~NetworkRequest();


protected:
    auto TBD1(string url)
    {
        gEnv->pSystem->GetIPluginManager()->QueryPlugin<Cry::Http::IPlugin>();
        //                                                   ^~~~ you what? Compiller not sayin any err.
    }
};


NetworkRequest::NetworkRequest(/* args */)
{
}
NetworkRequest::~NetworkRequest()
{
}



} // namespace tools
} // namespace ceblanktc1
