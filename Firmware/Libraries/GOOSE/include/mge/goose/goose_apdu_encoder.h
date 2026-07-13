/**
 * @file goose_apdu_encoder.h
 * @brief Codificador da APDU IEC 61850 GOOSE.
 */
#pragma once

#include "mge/ber/ber_encoder.h"
#include "mge/goose/goose_message.h"

namespace mge { namespace goose {

class ApduEncoder
{
public:
    /**
     * Codifica a APDU GOOSE (tag Application 1 / 0x61).
     *
     * Os buffers de trabalho são fornecidos pelo chamador para evitar
     * alocação dinâmica e consumo oculto de pilha no firmware embarcado.
     */
    template <core::usize Capacity>
    static core::Result encode(
        const Message<Capacity>& message,
        core::BufferWriter& output,
        core::MutableBufferView apduContentScratch,
        core::MutableBufferView allDataScratch)
    {
        if (!message.valid())
            return core::Error::InvalidState;

        core::BufferWriter contentWriter(apduContentScratch);
        core::Result result = encodeFields(message, contentWriter, allDataScratch);
        if (result.failed()) return result;

        ber::Encoder outer(output);
        const ber::Tag gooseApdu{
            ber::TagClass::Application,
            ber::PcBit::Constructed,
            1};

        return outer.writeConstructed(
            gooseApdu,
            core::BufferView(apduContentScratch.data(), contentWriter.position()));
    }

private:
    template <core::usize Capacity>
    static core::Result encodeFields(
        const Message<Capacity>& message,
        core::BufferWriter& writer,
        core::MutableBufferView allDataScratch)
    {
        const ControlBlock& control = message.controlBlock();

        core::Result result = writeContextBytes(
            writer, 0, ber::PcBit::Primitive,
            control.goCbRef(), textLength(control.goCbRef()));
        if (result.failed()) return result;

        result = writeContextUnsigned(writer, 1, control.timeAllowedToLiveMs());
        if (result.failed()) return result;

        result = writeContextBytes(
            writer, 2, ber::PcBit::Primitive,
            control.dataSetReference(), textLength(control.dataSetReference()));
        if (result.failed()) return result;

        result = writeContextBytes(
            writer, 3, ber::PcBit::Primitive,
            control.goId(), textLength(control.goId()));
        if (result.failed()) return result;

        result = writeContextBytes(
            writer, 4, ber::PcBit::Primitive,
            message.timestamp().data(), message.timestamp().size());
        if (result.failed()) return result;

        result = writeContextUnsigned(writer, 5, message.stateNumber());
        if (result.failed()) return result;

        result = writeContextUnsigned(writer, 6, message.sequenceNumber());
        if (result.failed()) return result;

        result = writeContextBoolean(writer, 7, message.test());
        if (result.failed()) return result;

        result = writeContextUnsigned(writer, 8, control.configurationRevision());
        if (result.failed()) return result;

        result = writeContextBoolean(writer, 9, message.needsCommissioning());
        if (result.failed()) return result;

        result = writeContextUnsigned(writer, 10, message.dataset().size());
        if (result.failed()) return result;

        core::BufferWriter allDataWriter(allDataScratch);
        result = encodeAllData(message.dataset(), allDataWriter);
        if (result.failed()) return result;

        return writeContextBytes(
            writer, 11, ber::PcBit::Constructed,
            allDataScratch.data(), allDataWriter.position());
    }

    template <core::usize Capacity>
    static core::Result encodeAllData(
        const Dataset<Capacity>& dataset,
        core::BufferWriter& writer)
    {
        for (core::usize i = 0; i < dataset.size(); ++i)
        {
            const asn1::Value* value = dataset.get(i);
            if (value == nullptr) return core::Error::InternalError;

            core::Result result = encodeDataValue(*value, writer);
            if (result.failed()) return result;
        }
        return core::Error::Ok;
    }

    static core::Result encodeDataValue(
        const asn1::Value& value,
        core::BufferWriter& writer);

    static core::Result writeContextBytes(
        core::BufferWriter& writer,
        core::u32 tagNumber,
        ber::PcBit pc,
        const void* data,
        core::usize size);

    static core::Result writeContextUnsigned(
        core::BufferWriter& writer,
        core::u32 tagNumber,
        core::u64 value);

    static core::Result writeContextSigned(
        core::BufferWriter& writer,
        core::u32 tagNumber,
        core::s64 value);

    static core::Result writeContextBoolean(
        core::BufferWriter& writer,
        core::u32 tagNumber,
        bool value);

    static core::usize textLength(const char* text) noexcept;
};

}} // namespace mge::goose
