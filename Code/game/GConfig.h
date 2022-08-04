#pragma once
#include "project.h"


namespace game
{


/**
 * this game config class
 * 
 * some rule of gameplay or data may be from this game config class
 * 
 */
class GConfig
{
public:
    const uint m_major = 0; // major versioning
    const uint m_minor = 0; // minor versioning
    const uint m_patch = 0; // patch versioning


    GConfig(/* args */) = default;
    virtual ~GConfig() = default;


    /**
     * @brief Get the Project Version object
     * 
     * @return string - not std::sring
     */
    string GetProjectVersion() const
    {
        string retval;

        std::string format = std::to_string(m_major) + "." + std::to_string(m_minor) + "." + std::to_string(m_patch);
        retval = format.c_str();

        return retval;
    }
};



} // namespace game
