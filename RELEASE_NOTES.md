# Sprint 004.1 — BER Foundation

## Arquivos adicionados

- `ber_tag.h`
- `ber_length.h`
- `ber_encoder.h`
- `ber_decoder.h`
- implementações correspondentes
- testes
- exemplo
- documentação
- CMake

## Observação de integração

O `BufferReader` precisa fornecer o método:

```cpp
const byte* currentData() const noexcept;
```

## Commit sugerido

```text
BER: adiciona foundation de Tag, Length, Encoder e Decoder
```
