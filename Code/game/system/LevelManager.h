#pragma once
#include "ceblanktc1.h"


namespace ceblanktc1
{
namespace game
{
namespace system
{


class LevelManager
{
public:
    LevelManager(/* args */) = default;
    virtual ~LevelManager() = default;


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
} // namespace ceblanktc1
