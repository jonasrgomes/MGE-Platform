/******************************************************************************
 *
 *  MGE Platform
 *  Industrial IEC 61850 Platform
 *
 ******************************************************************************
 *
 * @file        types.h
 * @brief       Tipos fundamentais utilizados por toda a plataforma.
 *
 * @author      Jonas Rodrigues Gomes
 * @version     0.1.0
 * @date        2026
 *
 * Requirements:
 *      REQ-CORE-001
 *
 *****************************************************************************/

#pragma once

#include <cstddef>
#include <cstdint>

namespace mge
{
namespace core
{

//=============================================================================
// Unsigned integer types
//=============================================================================

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

//=============================================================================
// Signed integer types
//=============================================================================

using s8  = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

//=============================================================================
// Floating point
//=============================================================================

using f32 = float;
using f64 = double;

//=============================================================================
// Generic byte
//=============================================================================

using byte = std::uint8_t;

//=============================================================================
// Size type
//=============================================================================

using usize = std::size_t;

//=============================================================================
// Compile-time verification
//=============================================================================

static_assert(sizeof(u8)  == 1, "u8 must be 1 byte");
static_assert(sizeof(u16) == 2, "u16 must be 2 bytes");
static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
static_assert(sizeof(u64) == 8, "u64 must be 8 bytes");

static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(f64) == 8, "f64 must be 8 bytes");

//=============================================================================
// Common constants
//=============================================================================

constexpr byte TRUE_BYTE  = 0xFF;
constexpr byte FALSE_BYTE = 0x00;

} // namespace core
} // namespace mge