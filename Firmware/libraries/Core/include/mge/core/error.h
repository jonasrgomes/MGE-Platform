/******************************************************************************
 *
 *  MGE Platform
 *  Industrial IEC 61850 Platform
 *
 *******************************************************************************
 *
 * @file        error.h
 * @brief       Error codes used throughout the MGE Platform.
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

/**
 * @brief Standard error codes.
 */
enum class Error
{
    Ok = 0,

    InvalidParameter,

    InvalidPointer,

    InvalidState,

    InvalidLength,

    InvalidTag,

    InvalidType,

    InvalidValue,

    InvalidAddress,

    BufferOverflow,

    BufferUnderflow,

    Timeout,

    NotFound,

    AlreadyExists,

    NotInitialized,

    Unsupported,

    CrcError,

    ChecksumError,

    DecodeError,

    EncodeError,

    CommunicationError,

    InternalError
};

/**
 * @brief Returns true when the operation completed successfully.
 */
constexpr bool success(Error error)
{
    return error == Error::Ok;
}

/**
 * @brief Returns true when the operation failed.
 */
constexpr bool failed(Error error)
{
    return error != Error::Ok;
}

} // namespace core
} // namespace mge