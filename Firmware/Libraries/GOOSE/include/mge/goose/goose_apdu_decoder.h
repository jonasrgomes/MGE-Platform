/**
 * @file goose_apdu_decoder.h
 * @brief Decoder zero-copy da APDU IEC 61850 GOOSE.
 */
#pragma once

#include "mge/ber/ber_decoder.h"
#include "mge/goose/goose_message.h"

namespace mge { namespace goose {

class ApduDecoder
{
public:
    template <core::usize Capacity>
    static core::Result decode(
        const core::BufferView& apdu,
        Message<Capacity>& message)
    {
        if (apdu.data() == nullptr)
            return core::Error::InvalidPointer;

        core::BufferReader outerReader(apdu);
        ber::Decoder outerDecoder(outerReader);
        ber::TlvView outer;

        core::Result result = outerDecoder.readTlv(outer);
        if (result.failed()) return result;

        const ber::Tag expected{
            ber::TagClass::Application,
            ber::PcBit::Constructed,
            1};

        if (outer.tag != expected)
            return core::Error::InvalidTag;

        if (!outerReader.eof())
            return core::Error::InvalidLength;

        return decodeFields(outer.value, message);
    }

private:
    template <core::usize Capacity>
    static core::Result decodeFields(
        const core::BufferView& content,
        Message<Capacity>& message)
    {
        core::BufferReader reader(content);
        ber::Decoder decoder(reader);

        bool seen[12]{};
        core::usize declaredEntries = 0;
        bool hasDeclaredEntries = false;

        message.dataset().clear();

        while (!reader.eof())
        {
            ber::TlvView field;
            core::Result result = decoder.readTlv(field);
            if (result.failed()) return result;

            if (field.tag.tagClass != ber::TagClass::ContextSpecific)
                return core::Error::InvalidTag;

            if (field.tag.number > 11U)
                return core::Error::Unsupported;

            if (seen[field.tag.number])
                return core::Error::AlreadyExists;

            seen[field.tag.number] = true;

            switch (field.tag.number)
            {
                case 0:
                    result = setText(field.value,
                                     message.controlBlock(),
                                     TextField::GoCbRef);
                    break;

                case 1:
                {
                    core::u64 value = 0;
                    result = decodeUnsigned(field.value, value);
                    if (result.ok())
                        result = message.controlBlock().setTimeAllowedToLiveMs(
                            static_cast<core::u32>(value));
                    break;
                }

                case 2:
                    result = setText(field.value,
                                     message.controlBlock(),
                                     TextField::DataSetReference);
                    break;

                case 3:
                    result = setText(field.value,
                                     message.controlBlock(),
                                     TextField::GoId);
                    break;

                case 4:
                {
                    Timestamp timestamp;
                    result = decodeTimestamp(field.value, timestamp);
                    if (result.ok()) message.setTimestamp(timestamp);
                    break;
                }

                case 5:
                {
                    core::u64 value = 0;
                    result = decodeUnsigned(field.value, value);
                    if (result.ok())
                        message.setStateNumber(static_cast<core::u32>(value));
                    break;
                }

                case 6:
                {
                    core::u64 value = 0;
                    result = decodeUnsigned(field.value, value);
                    if (result.ok())
                        message.setSequenceNumber(static_cast<core::u32>(value));
                    break;
                }

                case 7:
                {
                    bool value = false;
                    result = decodeBoolean(field.value, value);
                    if (result.ok()) message.setTest(value);
                    break;
                }

                case 8:
                {
                    core::u64 value = 0;
                    result = decodeUnsigned(field.value, value);
                    if (result.ok())
                        result = message.controlBlock().setConfigurationRevision(
                            static_cast<core::u32>(value));
                    break;
                }

                case 9:
                {
                    bool value = false;
                    result = decodeBoolean(field.value, value);
                    if (result.ok()) message.setNeedsCommissioning(value);
                    break;
                }

                case 10:
                {
                    core::u64 value = 0;
                    result = decodeUnsigned(field.value, value);
                    if (result.ok())
                    {
                        if (value > Capacity)
                            return core::Error::BufferOverflow;
                        declaredEntries = static_cast<core::usize>(value);
                        hasDeclaredEntries = true;
                    }
                    break;
                }

                case 11:
                    if (field.tag.pc != ber::PcBit::Constructed)
                        return core::Error::InvalidType;
                    result = decodeAllData(field.value, message.dataset());
                    break;

                default:
                    result = core::Error::Unsupported;
                    break;
            }

            if (result.failed()) return result;
        }

        for (core::usize i = 0; i < 12; ++i)
            if (!seen[i]) return core::Error::NotFound;

        if (!hasDeclaredEntries || declaredEntries != message.dataset().size())
            return core::Error::InvalidLength;

        return message.valid()
            ? core::Error::Ok
            : core::Error::InvalidState;
    }

    template <core::usize Capacity>
    static core::Result decodeAllData(
        const core::BufferView& bytes,
        Dataset<Capacity>& dataset)
    {
        core::BufferReader reader(bytes);
        ber::Decoder decoder(reader);

        while (!reader.eof())
        {
            ber::TlvView value;
            core::Result result = decoder.readTlv(value);
            if (result.failed()) return result;

            if (value.tag.tagClass != ber::TagClass::ContextSpecific ||
                value.tag.pc != ber::PcBit::Primitive)
                return core::Error::InvalidTag;

            switch (value.tag.number)
            {
                case 3:
                {
                    bool decoded = false;
                    result = decodeBoolean(value.value, decoded);
                    if (result.ok()) result = dataset.addBoolean(decoded);
                    break;
                }

                case 4:
                {
                    if (value.value.size() < 1)
                        return core::Error::InvalidLength;

                    const core::u8 unusedBits = value.value[0];
                    if (unusedBits > 7U)
                        return core::Error::InvalidValue;

                    const core::BufferView payload =
                        value.value.subview(1, value.value.size() - 1);

                    const core::usize bitCount =
                        payload.size() * 8U - unusedBits;

                    result = dataset.addBitString(payload, bitCount);
                    break;
                }

                case 5:
                {
                    core::s64 decoded = 0;
                    result = decodeSigned(value.value, decoded);
                    if (result.ok()) result = dataset.addInteger(decoded);
                    break;
                }

                case 9:
                    result = dataset.addOctetString(value.value);
                    break;

                case 10:
                    result = dataset.addVisibleString(value.value);
                    break;

                case 16:
                    result = dataset.addUtf8String(value.value);
                    break;

                default:
                    return core::Error::Unsupported;
            }

            if (result.failed()) return result;
        }

        return core::Error::Ok;
    }

    enum class TextField
    {
        GoCbRef,
        DataSetReference,
        GoId
    };

    static core::Result setText(
        const core::BufferView& value,
        ControlBlock& control,
        TextField field);

    static core::Result decodeUnsigned(
        const core::BufferView& bytes,
        core::u64& value) noexcept;

    static core::Result decodeSigned(
        const core::BufferView& bytes,
        core::s64& value) noexcept;

    static core::Result decodeBoolean(
        const core::BufferView& bytes,
        bool& value) noexcept;

    static core::Result decodeTimestamp(
        const core::BufferView& bytes,
        Timestamp& timestamp) noexcept;
};

}} // namespace mge::goose
