#pragma once
#include "project.h"


namespace game
{
namespace system
{


class SLevelManager
{
public:
    SLevelManager(/* args */) = default;
    virtual ~SLevelManager() = default;


    /**
     * @brief Get the Current Level object
     * 
     * @return const string 
     */
    const static string GetCurrentLevel()
    {
        return gEnv->pConsole->GetCVar("sv_map")->GetString();
    }
};



} // namespace system
} // namespace game
