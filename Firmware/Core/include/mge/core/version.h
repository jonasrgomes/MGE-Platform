/******************************************************************************
 *
 *  MGE Platform
 *  Industrial Automation Framework
 *
 *******************************************************************************
 *
 * @file        version.h
 * @brief       Version information for the MGE Platform.
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

//=============================================================================
// Version
//=============================================================================

constexpr std::uint16_t VERSION_MAJOR = 0;
constexpr std::uint16_t VERSION_MINOR = 1;
constexpr std::uint16_t VERSION_PATCH = 0;

//=============================================================================
// Build information
//=============================================================================

constexpr const char* VERSION_STRING = "0.1.0";

constexpr const char* PLATFORM_NAME =
    "MGE Platform";

constexpr const char* PLATFORM_DESCRIPTION =
    "Industrial Automation Framework";

} // namespace core
} // namespace mge