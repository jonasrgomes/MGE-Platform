#include "mge/goose/goose_apdu_decoder.h"

namespace mge { namespace goose {

core::Result ApduDecoder::setText(
    const core::BufferView& value,
    ControlBlock& control,
    TextField field)
{
    const core::usize capacity =
        field == TextField::GoId
            ? MaxGoIdLength
            : MaxObjectReferenceLength;

    if (value.size() >= capacity)
        return core::Error::InvalidLength;

    char text[MaxObjectReferenceLength]{};

    for (core::usize i = 0; i < value.size(); ++i)
    {
        if (value[i] == 0)
            return core::Error::InvalidValue;
        text[i] = static_cast<char>(value[i]);
    }

    switch (field)
    {
        case TextField::GoCbRef:
            return control.setGoCbRef(text);

        case TextField::DataSetReference:
            return control.setDataSetReference(text);

        case TextField::GoId:
            return control.setGoId(text);
    }

    return core::Error::InternalError;
}

core::Result ApduDecoder::decodeUnsigned(
    const core::BufferView& bytes,
    core::u64& value) noexcept
{
    if (bytes.data() == nullptr)
        return core::Error::InvalidPointer;

    if (bytes.empty() || bytes.size() > 9)
        return core::Error::InvalidLength;

    core::usize start = 0;

    if (bytes.size() == 9)
    {
        if (bytes[0] != 0)
            return core::Error::InvalidValue;
        start = 1;
    }

    if (start < bytes.size() && (bytes[start] & 0x80U) != 0)
        return core::Error::InvalidValue;

    value = 0;

    for (core::usize i = start; i < bytes.size(); ++i)
        value = (value << 8) | bytes[i];

    return core::Error::Ok;
}

core::Result ApduDecoder::decodeSigned(
    const core::BufferView& bytes,
    core::s64& value) noexcept
{
    if (bytes.data() == nullptr)
        return core::Error::InvalidPointer;

    if (bytes.empty() || bytes.size() > 8)
        return core::Error::InvalidLength;

    core::u64 raw = 0;

    for (core::usize i = 0; i < bytes.size(); ++i)
        raw = (raw << 8) | bytes[i];

    if ((bytes[0] & 0x80U) != 0 && bytes.size() < 8)
        raw |= (~core::u64{0}) << (bytes.size() * 8U);

    value = static_cast<core::s64>(raw);
    return core::Error::Ok;
}

core::Result ApduDecoder::decodeBoolean(
    const core::BufferView& bytes,
    bool& value) noexcept
{
    if (bytes.size() != 1)
        return core::Error::InvalidLength;

    value = bytes[0] != 0;
    return core::Error::Ok;
}

core::Result ApduDecoder::decodeTimestamp(
    const core::BufferView& bytes,
    Timestamp& timestamp) noexcept
{
    if (bytes.size() != TimestampSize)
        return core::Error::InvalidLength;

    core::byte copied[TimestampSize]{};
    for (core::usize i = 0; i < TimestampSize; ++i)
        copied[i] = bytes[i];

    timestamp = Timestamp(copied);
    return core::Error::Ok;
}

}} // namespace mge::goose
