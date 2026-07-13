# GOOSE Foundation — Sprint 006.1A

Esta entrega cria o modelo de dados da biblioteca GOOSE, ainda sem codificar a APDU.

## Componentes

- `Header`: APPID, Length, Reserved1 e Reserved2.
- `ControlBlock`: endereço multicast, VLAN, TTL, confRev e referências IEC 61850.
- `Dataset`: adaptação estática do dataset ASN.1 para `allData`.
- `Message`: estado, sequência, timestamp, flags e dataset.

## Segurança

- sem alocação dinâmica;
- textos armazenados em arrays fixos;
- validação de APPID, VLAN e MAC multicast;
- saídas e transmissão não são acessadas nesta Sprint.

## Próxima parte

A Sprint 006.1B adicionará as tags context-specific da APDU GOOSE e o encoder dos campos:

- `gocbRef`;
- `timeAllowedToLive`;
- `datSet`;
- `goID`;
- `t`;
- `stNum`;
- `sqNum`;
- `test`;
- `confRev`;
- `ndsCom`;
- `numDatSetEntries`;
- `allData`.
