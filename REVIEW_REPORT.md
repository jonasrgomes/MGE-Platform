# Relatório de revisão — Sprint 003.9

## Correções realizadas

- Consolidada a estrutura oficial em `Firmware/Libraries`.
- Removidas da entrega as estruturas duplicadas `Firmware/MGE-*`, `Firmware/Core` e `Firmware/libraries`.
- Corrigido `plataform.h` para `platform.h`.
- Criado `endian.h` compartilhado por Reader e Writer.
- Adicionado `BufferReader::currentData()` para o decoder BER zero-copy.
- Adicionado acesso indexado seguro em `BufferView`.
- Completados `EthernetFrame` e `EthernetBuilder`, que estavam ausentes no ZIP revisado.
- Corrigida a enumeração de erros com `InvalidTag` e `InvalidType`.
- Reorganizados os arquivos CMake da raiz, Core, Ethernet e BER.
- Removida a classe `Buffer` incompleta; a API oficial nesta fase é formada por `BufferView`, `BufferReader` e `BufferWriter`.
- Removido o diretório `.git` da entrega.

## Validação executada

O projeto foi compilado com GCC/C++17 e os seguintes testes foram executados:

1. `buffer_reader_test`
2. `buffer_writer_test`
3. `mac_address_test`
4. `ethernet_builder_test`
5. `ethernet_parser_test`
6. `ber_foundation_test`
7. `ber_primitive_types_test`

Resultado: **7 de 7 testes aprovados**.

## Estrutura oficial

```text
Firmware/
├── Libraries/
│   ├── Core/
│   ├── Ethernet/
│   ├── BER/
│   ├── ASN1/
│   ├── GOOSE/
│   ├── Logic/
│   ├── IED/
│   └── W5500/
├── Drivers/
└── Applications/
```
