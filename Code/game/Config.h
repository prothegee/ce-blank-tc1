#pragma once
#include "ceblanktc1.h"


namespace ceblanktc1
{
namespace game
{


/**
 * this game config class
 * 
 * some rule of gameplay or data may be from this game config class
 * 
 */
class Config
{
public:
    const static uint m_major = 0; // major versioning
    const static uint m_minor = 0; // minor versioning
    const static uint m_patch = 0; // patch versioning


    Config(/* args */);
    virtual ~Config();


    /**
     * @brief Get the Project Version object
     * 
     * @return string - not std::sring
     */
    const static string GetProjectVersion()
    {
        string retval;

        std::string format = std::to_string(m_major) + "." + std::to_string(m_minor) + "." + std::to_string(m_patch);
        retval = format.c_str();

        return retval;
    }
};



} // namespace game
} // namespace ceblanktc1
