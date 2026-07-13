/**
 * @file goose_control_block.h
 * @brief Configuração estática de um GOOSE Control Block.
 */
#pragma once

#include <cstring>
#include "mge/core/result.h"
#include "mge/ethernet/mac_address.h"
#include "mge/goose/goose_constants.h"

namespace mge { namespace goose {

class ControlBlock
{
public:
    ControlBlock() noexcept;

    core::u16 appId() const noexcept { return m_appId; }
    const ethernet::MacAddress& destinationMac() const noexcept { return m_destinationMac; }
    core::u16 vlanId() const noexcept { return m_vlanId; }
    core::u8 vlanPriority() const noexcept { return m_vlanPriority; }
    core::u32 timeAllowedToLiveMs() const noexcept { return m_timeAllowedToLiveMs; }
    core::u32 configurationRevision() const noexcept { return m_configurationRevision; }
    const char* goCbRef() const noexcept { return m_goCbRef; }
    const char* dataSetReference() const noexcept { return m_dataSetReference; }
    const char* goId() const noexcept { return m_goId; }

    core::Result setAppId(core::u16 value) noexcept;
    core::Result setDestinationMac(const ethernet::MacAddress& value) noexcept;
    core::Result setVlan(core::u16 id, core::u8 priority) noexcept;
    core::Result setTimeAllowedToLiveMs(core::u32 value) noexcept;
    core::Result setConfigurationRevision(core::u32 value) noexcept;
    core::Result setGoCbRef(const char* value) noexcept;
    core::Result setDataSetReference(const char* value) noexcept;
    core::Result setGoId(const char* value) noexcept;

    bool valid() const noexcept;

private:
    static core::Result copyText(char* destination,
                                 core::usize capacity,
                                 const char* source) noexcept;

    core::u16 m_appId = 0;
    ethernet::MacAddress m_destinationMac{};
    core::u16 m_vlanId = DefaultVlanId;
    core::u8 m_vlanPriority = DefaultVlanPriority;
    core::u32 m_timeAllowedToLiveMs = DefaultTimeAllowedToLiveMs;
    core::u32 m_configurationRevision = DefaultConfigurationRevision;
    char m_goCbRef[MaxObjectReferenceLength]{};
    char m_dataSetReference[MaxObjectReferenceLength]{};
    char m_goId[MaxGoIdLength]{};
};

}} // namespace mge::goose
