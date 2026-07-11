/******************************************************************************
 *
 *  MGE Platform
 *  Industrial Automation Framework
 *
 *******************************************************************************
 *
 * @file        config.h
 * @brief       Global configuration parameters.
 *
 * @author      Jonas Rodrigues Gomes
 * @version     0.1.0
 *
 ******************************************************************************/

#pragma once

namespace mge
{
namespace core
{

//=============================================================================
// General configuration
//=============================================================================

constexpr bool ENABLE_LOGGER = true;

constexpr bool ENABLE_ASSERT = true;

constexpr bool ENABLE_DEBUG = true;

constexpr bool ENABLE_STATISTICS = true;

//=============================================================================
// Buffer configuration
//=============================================================================

constexpr unsigned DEFAULT_BUFFER_SIZE = 2048;

constexpr unsigned MAX_ETHERNET_FRAME = 1518;

constexpr unsigned MAX_GOOSE_FRAME = 1518;

//=============================================================================
// Timeouts
//=============================================================================

constexpr unsigned DEFAULT_TIMEOUT_MS = 1000;

constexpr unsigned SOCKET_TIMEOUT_MS = 500;

//=============================================================================
// Task configuration
//=============================================================================

constexpr unsigned DEFAULT_STACK_SIZE = 4096;

constexpr unsigned DEFAULT_PRIORITY = 5;

} // namespace core
} // namespace mge