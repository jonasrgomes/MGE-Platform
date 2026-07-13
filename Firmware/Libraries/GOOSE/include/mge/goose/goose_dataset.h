/**
 * @file goose_dataset.h
 * @brief Dataset estático usado no campo allData do GOOSE.
 */
#pragma once

#include "mge/asn1/asn1_dataset.h"
#include "mge/goose/goose_constants.h"

namespace mge { namespace goose {

template <core::usize Capacity = DefaultDatasetCapacity>
class Dataset
{
public:
    core::usize size() const noexcept { return m_values.size(); }
    static constexpr core::usize capacity() noexcept { return Capacity; }
    bool empty() const noexcept { return m_values.empty(); }
    bool full() const noexcept { return m_values.full(); }
    void clear() noexcept { m_values.clear(); }

    core::Result addNull() noexcept { return m_values.addNull(); }
    core::Result addBoolean(bool value) noexcept { return m_values.addBoolean(value); }
    core::Result addInteger(core::s64 value) noexcept { return m_values.addInteger(value); }
    core::Result addOctetString(core::BufferView value) noexcept
    { return m_values.addOctetString(value); }
    core::Result addVisibleString(core::BufferView value) noexcept
    { return m_values.addVisibleString(value); }
    core::Result addUtf8String(core::BufferView value) noexcept
    { return m_values.addUtf8String(value); }
    core::Result addBitString(core::BufferView bytes, core::usize bitCount) noexcept
    { return m_values.addBitString(bytes, bitCount); }

    const asn1::Value* get(core::usize index) const noexcept
    { return m_values.get(index); }

    asn1::Value* get(core::usize index) noexcept
    { return m_values.get(index); }

    const asn1::Dataset<Capacity>& asn1Dataset() const noexcept
    { return m_values; }

    asn1::Dataset<Capacity>& asn1Dataset() noexcept
    { return m_values; }

private:
    asn1::Dataset<Capacity> m_values{};
};

}} // namespace mge::goose
