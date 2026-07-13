# Sprint 006.1A — GOOSE Foundation

## Implementado

- constantes GOOSE;
- `Timestamp` IEC 61850 de oito octetos;
- cabeçalho GOOSE de 8 bytes;
- `ControlBlock` com memória estática;
- APPID;
- MAC multicast;
- VLAN ID e prioridade;
- `timeAllowedToLive`;
- `confRev`;
- `gocbRef`, `datSet` e `goID`;
- `Dataset<Capacity>` baseado no ASN.1 Dataset;
- `Message<Capacity>`;
- controle inicial de `stNum` e `sqNum`;
- testes e exemplo;
- integração ao CMake principal.

## Validação

O projeto completo foi compilado em C++17.

```text
12 de 12 testes aprovados
0 falhas
```

## Limite desta Sprint

Esta entrega ainda não codifica a APDU GOOSE e não transmite pela Ethernet.

## Integração

Extraia o ZIP na raiz do repositório e aceite a substituição do
`CMakeLists.txt` principal e do módulo `Firmware/Libraries/GOOSE`.

## Commit sugerido

```text
GOOSE: adiciona foundation de Header, ControlBlock, Dataset e Message
```

## Próxima entrega

Sprint 006.1B — GOOSE APDU Tags e Encoder Base.
