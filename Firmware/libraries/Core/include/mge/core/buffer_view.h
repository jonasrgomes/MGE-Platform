/******************************************************************************
 *
 * MGE Platform
 * Industrial Automation Framework
 *
 * @file        buffer_view.h
 * @brief       Visões não proprietárias sobre regiões de memória.
 * @author      Jonas Rodrigues Gomes
 * @version     0.1.0
 *
 * Requirements:
 *      REQ-CORE-BUFFER-001
 *
 ******************************************************************************/

#pragma once

#include "types.h"

namespace mge
{
namespace core
{

/**
 * @brief Visão somente leitura sobre uma região de bytes.
 *
 * A classe não possui nem libera a memória apontada.
 * O chamador deve garantir que a região continue válida.
 */
class BufferView
{
public:

    /**
     * @brief Cria uma visão vazia.
     */
    constexpr BufferView() noexcept
        : m_data(nullptr),
          m_size(0)
    {
    }

    /**
     * @brief Cria uma visão somente leitura.
     *
     * @param data Ponteiro para o primeiro byte.
     * @param size Quantidade de bytes disponíveis.
     */
    constexpr BufferView(const byte* data, usize size) noexcept
        : m_data(data),
          m_size(data != nullptr ? size : 0)
    {
    }

    /**
     * @brief Retorna o ponteiro para o primeiro byte.
     */
    constexpr const byte* data() const noexcept
    {
        return m_data;
    }

    /**
     * @brief Retorna a quantidade de bytes da visão.
     */
    constexpr usize size() const noexcept
    {
        return m_size;
    }

    constexpr byte operator[](usize index) const noexcept
    {
        return (m_data != nullptr && index < m_size) ? m_data[index] : 0;
    }

    /**
     * @brief Informa se a visão está vazia.
     */
    constexpr bool empty() const noexcept
    {
        return m_size == 0;
    }

    /**
     * @brief Verifica se um intervalo pertence à visão.
     *
     * @param offset Posição inicial.
     * @param length Quantidade de bytes.
     */
    constexpr bool contains(usize offset, usize length) const noexcept
    {
        return offset <= m_size &&
               length <= (m_size - offset);
    }

    /**
     * @brief Obtém um byte sem alterar a visão.
     *
     * @param index Índice do byte.
     * @param value Recebe o valor.
     * @return true quando o índice é válido.
     */
    constexpr bool get(usize index, byte& value) const noexcept
    {
        if (index >= m_size || m_data == nullptr)
        {
            return false;
        }

        value = m_data[index];
        return true;
    }

    /**
     * @brief Cria uma subvisão sem copiar dados.
     *
     * Retorna uma visão vazia quando o intervalo for inválido.
     */
    constexpr BufferView subview(usize offset,
                                 usize length) const noexcept
    {
        if (!contains(offset, length) || m_data == nullptr)
        {
            return {};
        }

        return BufferView(m_data + offset, length);
    }

private:

    const byte* m_data;
    usize m_size;
};

/**
 * @brief Visão de leitura e escrita sobre uma região de bytes.
 *
 * A classe não possui nem libera a memória apontada.
 */
class MutableBufferView
{
public:

    /**
     * @brief Cria uma visão vazia.
     */
    constexpr MutableBufferView() noexcept
        : m_data(nullptr),
          m_size(0)
    {
    }

    /**
     * @brief Cria uma visão mutável.
     *
     * @param data Ponteiro para o primeiro byte.
     * @param size Quantidade de bytes disponíveis.
     */
    constexpr MutableBufferView(byte* data, usize size) noexcept
        : m_data(data),
          m_size(data != nullptr ? size : 0)
    {
    }

    constexpr byte* data() noexcept
    {
        return m_data;
    }

    constexpr const byte* data() const noexcept
    {
        return m_data;
    }

    constexpr usize size() const noexcept
    {
        return m_size;
    }

    constexpr bool empty() const noexcept
    {
        return m_size == 0;
    }

    constexpr bool contains(usize offset, usize length) const noexcept
    {
        return offset <= m_size &&
               length <= (m_size - offset);
    }

    /**
     * @brief Escreve um byte em uma posição específica.
     */
    constexpr bool set(usize index, byte value) noexcept
    {
        if (index >= m_size || m_data == nullptr)
        {
            return false;
        }

        m_data[index] = value;
        return true;
    }

    /**
     * @brief Lê um byte em uma posição específica.
     */
    constexpr bool get(usize index, byte& value) const noexcept
    {
        if (index >= m_size || m_data == nullptr)
        {
            return false;
        }

        value = m_data[index];
        return true;
    }

    /**
     * @brief Cria uma subvisão mutável sem copiar dados.
     */
    constexpr MutableBufferView subview(usize offset,
                                        usize length) noexcept
    {
        if (!contains(offset, length) || m_data == nullptr)
        {
            return {};
        }

        return MutableBufferView(m_data + offset, length);
    }

    /**
     * @brief Converte para uma visão somente leitura.
     */
    constexpr BufferView asReadOnly() const noexcept
    {
        return BufferView(m_data, m_size);
    }

private:

    byte* m_data;
    usize m_size;
};

} // namespace core
} // namespace mge