/**
 * @file goose_message.h
 * @brief Estado completo de uma mensagem GOOSE antes da codificação.
 */
#pragma once

#include "mge/goose/goose_control_block.h"
#include "mge/goose/goose_dataset.h"
#include "mge/goose/goose_header.h"
#include "mge/goose/goose_types.h"

namespace mge { namespace goose {

template <core::usize DatasetCapacity = DefaultDatasetCapacity>
class Message
{
public:
    Header& header() noexcept { return m_header; }
    const Header& header() const noexcept { return m_header; }

    ControlBlock& controlBlock() noexcept { return m_controlBlock; }
    const ControlBlock& controlBlock() const noexcept { return m_controlBlock; }

    Dataset<DatasetCapacity>& dataset() noexcept { return m_dataset; }
    const Dataset<DatasetCapacity>& dataset() const noexcept { return m_dataset; }

    core::u32 stateNumber() const noexcept { return m_stateNumber; }
    core::u32 sequenceNumber() const noexcept { return m_sequenceNumber; }
    const Timestamp& timestamp() const noexcept { return m_timestamp; }
    bool test() const noexcept { return m_test; }
    bool needsCommissioning() const noexcept { return m_needsCommissioning; }

    void setStateNumber(core::u32 value) noexcept { m_stateNumber = value; }
    void setSequenceNumber(core::u32 value) noexcept { m_sequenceNumber = value; }
    void setTimestamp(const Timestamp& value) noexcept { m_timestamp = value; }
    void setTest(bool value) noexcept { m_test = value; }
    void setNeedsCommissioning(bool value) noexcept { m_needsCommissioning = value; }

    void markStateChange() noexcept
    {
        ++m_stateNumber;
        m_sequenceNumber = 0;
    }

    void markRetransmission() noexcept
    {
        ++m_sequenceNumber;
    }

    bool valid() const noexcept
    {
        return m_controlBlock.valid() && !m_dataset.empty();
    }

private:
    Header m_header{};
    ControlBlock m_controlBlock{};
    Dataset<DatasetCapacity> m_dataset{};
    Timestamp m_timestamp{};
    core::u32 m_stateNumber = 1;
    core::u32 m_sequenceNumber = 0;
    bool m_test = false;
    bool m_needsCommissioning = false;
};

}} // namespace mge::goose
