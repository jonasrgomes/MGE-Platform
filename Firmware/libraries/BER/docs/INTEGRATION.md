# Integração da Sprint 004.1

Extraia este ZIP na raiz do repositório.

## Dependências

A biblioteca BER depende de:

```text
Firmware/Libraries/Core/include
```

O Core deve possuir:

- `BufferView`;
- `BufferReader`;
- `BufferWriter`;
- `Result`;
- `Error`;
- `types.h`.

## Ajuste necessário no BufferReader

O decoder usa:

```cpp
const byte* currentData() const noexcept;
```

Caso esse método ainda não exista, adicione ao `buffer_reader.h`:

```cpp
const byte* currentData() const noexcept;
```

E ao `buffer_reader.cpp`:

```cpp
const byte* BufferReader::currentData() const noexcept
{
    return m_buffer.data() + m_position;
}
```
