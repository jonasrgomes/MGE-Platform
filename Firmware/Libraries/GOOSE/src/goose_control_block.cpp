#include "mge/goose/goose_control_block.h"

namespace mge { namespace goose {

ControlBlock::ControlBlock() noexcept
    : m_destinationMac(0x01, 0x0C, 0xCD, 0x01, 0x00, 0x01)
{
}

core::Result ControlBlock::copyText(char* destination,
                                    core::usize capacity,
                                    const char* source) noexcept
{
    if (destination == nullptr || source == nullptr)
        return core::Error::InvalidPointer;

    const core::usize length = std::strlen(source);
    if (length == 0 || length >= capacity)
        return core::Error::InvalidLength;

    std::memcpy(destination, source, length + 1);
    return core::Error::Ok;
}

core::Result ControlBlock::setAppId(core::u16 value) noexcept
{
    if (value == 0) return core::Error::InvalidValue;
    m_appId = value;
    return core::Error::Ok;
}

core::Result ControlBlock::setDestinationMac(
    const ethernet::MacAddress& value) noexcept
{
    if (!value.isMulticast()) return core::Error::InvalidAddress;
    m_destinationMac = value;
    return core::Error::Ok;
}

core::Result ControlBlock::setVlan(core::u16 id,
                                   core::u8 priority) noexcept
{
    if (id > 4094 || priority > 7)
        return core::Error::InvalidValue;

    m_vlanId = id;
    m_vlanPriority = priority;
    return core::Error::Ok;
}

core::Result ControlBlock::setTimeAllowedToLiveMs(core::u32 value) noexcept
{
    if (value == 0) return core::Error::InvalidValue;
    m_timeAllowedToLiveMs = value;
    return core::Error::Ok;
}

core::Result ControlBlock::setConfigurationRevision(core::u32 value) noexcept
{
    if (value == 0) return core::Error::InvalidValue;
    m_configurationRevision = value;
    return core::Error::Ok;
}

core::Result ControlBlock::setGoCbRef(const char* value) noexcept
{
    return copyText(m_goCbRef, sizeof(m_goCbRef), value);
}

core::Result ControlBlock::setDataSetReference(const char* value) noexcept
{
    return copyText(m_dataSetReference, sizeof(m_dataSetReference), value);
}

core::Result ControlBlock::setGoId(const char* value) noexcept
{
    return copyText(m_goId, sizeof(m_goId), value);
}

bool ControlBlock::valid() const noexcept
{
    return m_appId != 0 &&
           m_destinationMac.isMulticast() &&
           m_timeAllowedToLiveMs != 0 &&
           m_configurationRevision != 0 &&
           m_goCbRef[0] != '\0' &&
           m_dataSetReference[0] != '\0' &&
           m_goId[0] != '\0';
}

}} // namespace mge::goose
