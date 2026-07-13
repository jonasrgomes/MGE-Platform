/**
 * @file goose_constants.h
 * @brief Constantes IEC 61850 GOOSE usadas pela MGE Platform.
 */
#pragma once

#include "mge/core/types.h"

namespace mge { namespace goose {

constexpr core::u16 EtherTypeValue = 0x88B8;
constexpr core::usize ProtocolHeaderSize = 8;
constexpr core::usize TimestampSize = 8;
constexpr core::usize DefaultDatasetCapacity = 32;
constexpr core::usize MaxObjectReferenceLength = 129;
constexpr core::usize MaxGoIdLength = 65;
constexpr core::u32 DefaultTimeAllowedToLiveMs = 1000;
constexpr core::u32 DefaultConfigurationRevision = 1;
constexpr core::u8 DefaultVlanPriority = 4;
constexpr core::u16 DefaultVlanId = 0;

}} // namespace mge::goose
