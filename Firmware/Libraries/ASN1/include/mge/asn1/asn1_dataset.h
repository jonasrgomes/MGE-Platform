/**
 * @file asn1_dataset.h
 * @brief Coleção estática de valores ASN.1 para datasets industriais.
 */
#pragma once

#include "mge/asn1/asn1_decoder.h"
#include "mge/asn1/asn1_encoder.h"

namespace mge { namespace asn1 {

template <core::usize Capacity>
class Dataset
{
public:
    static constexpr core::usize capacityValue = Capacity;

    constexpr Dataset() noexcept = default;

    constexpr core::usize size() const noexcept { return m_size; }
    static constexpr core::usize capacity() noexcept { return Capacity; }
    constexpr bool empty() const noexcept { return m_size == 0; }
    constexpr bool full() const noexcept { return m_size >= Capacity; }

    constexpr void clear() noexcept { m_size = 0; }

    core::Result add(const Value& value) noexcept
    {
        if (full()) return core::Error::BufferOverflow;
        m_values[m_size++] = value;
        return core::Error::Ok;
    }

    core::Result addNull() noexcept { return add(Value::null()); }
    core::Result addBoolean(bool value) noexcept { return add(Value::boolean(value)); }
    core::Result addInteger(std::int64_t value) noexcept { return add(Value::integer(value)); }
    core::Result addOctetString(core::BufferView value) noexcept
    { return add(Value::octetString(value)); }
    core::Result addVisibleString(core::BufferView value) noexcept
    { return add(Value::visibleString(value)); }
    core::Result addUtf8String(core::BufferView value) noexcept
    { return add(Value::utf8String(value)); }
    core::Result addBitString(core::BufferView bytes, core::usize bitCount) noexcept
    { return add(Value::bitString(bytes, bitCount)); }

    const Value* get(core::usize index) const noexcept
    {
        return index < m_size ? &m_values[index] : nullptr;
    }

    Value* get(core::usize index) noexcept
    {
        return index < m_size ? &m_values[index] : nullptr;
    }

    const Value& operator[](core::usize index) const noexcept
    {
        return m_values[index];
    }

    Value& operator[](core::usize index) noexcept
    {
        return m_values[index];
    }

private:
    Value m_values[Capacity == 0 ? 1 : Capacity]{};
    core::usize m_size = 0;
};

class DatasetEncoder
{
public:
    template <core::usize Capacity>
    static core::Result encode(
        const Dataset<Capacity>& dataset,
        Encoder& encoder)
    {
        for (core::usize i = 0; i < dataset.size(); ++i)
        {
            const core::Result result = encoder.encode(dataset[i]);
            if (result.failed()) return result;
        }
        return core::Error::Ok;
    }
};

class DatasetDecoder
{
public:
    template <core::usize Capacity>
    static core::Result decode(
        ber::Decoder& berDecoder,
        core::usize entryCount,
        Dataset<Capacity>& dataset)
    {
        if (entryCount > Capacity)
            return core::Error::BufferOverflow;

        dataset.clear();

        for (core::usize i = 0; i < entryCount; ++i)
        {
            ber::TlvView tlv;
            core::Result result = berDecoder.readTlv(tlv);
            if (result.failed()) return result;

            Value value;
            result = Decoder::decode(tlv, value);
            if (result.failed()) return result;

            result = dataset.add(value);
            if (result.failed()) return result;
        }

        return core::Error::Ok;
    }
};

}} // namespace mge::asn1
