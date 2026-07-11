/******************************************************************************
 *
 *  MGE Platform
 *  Industrial Automation Framework
 *
 *******************************************************************************
 *
 * @file        platform.h
 * @brief       Platform detection.
 *
 * @author      Jonas Rodrigues Gomes
 * @version     0.1.0
 *
 ******************************************************************************/

#pragma once

#include <cstdint>

namespace mge
{
namespace core
{

class Platform
{
public:

    static constexpr const char* name()
    {
#if defined(ESP_PLATFORM)
        return "ESP32";

#elif defined(_WIN32)
        return "Windows";

#elif defined(__linux__)
        return "Linux";

#elif defined(STM32F103xB) || defined(STM32F4)
        return "STM32";

#else
        return "Unknown";
#endif
    }

    static constexpr bool isESP32()
    {
#ifdef ESP_PLATFORM
        return true;
#else
        return false;
#endif
    }

    static constexpr bool isWindows()
    {
#ifdef _WIN32
        return true;
#else
        return false;
#endif
    }

    static constexpr bool isLinux()
    {
#ifdef __linux__
        return true;
#else
        return false;
#endif
    }

    static constexpr bool isSTM32()
    {
#if defined(STM32F103xB) || defined(STM32F4)
        return true;
#else
        return false;
#endif
    }

    static constexpr std::uint32_t pointerSize()
    {
        return sizeof(void*);
    }

};

} // namespace core
} // namespace mge