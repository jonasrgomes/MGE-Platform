/******************************************************************************
 *
 *  MGE Platform
 *  Industrial IEC 61850 Platform
 *
 *******************************************************************************
 *
 * @file        result.h
 * @brief       Generic result wrapper.
 *
 * @author      Jonas Rodrigues Gomes
 * @version     0.1.0
 *
 ******************************************************************************/

#pragma once

#include "error.h"

namespace mge
{
namespace core
{

/**
 * @brief Generic operation result.
 */
struct Result
{
    Error error;

    constexpr Result(Error e = Error::Ok)
        : error(e)
    {
    }

    constexpr bool ok() const
    {
        return error == Error::Ok;
    }

    constexpr bool failed() const
    {
        return error != Error::Ok;
    }

    constexpr operator bool() const
    {
        return ok();
    }
};

} // namespace core
} // namespace mge