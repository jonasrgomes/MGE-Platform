# GOOSE Frame Encoder

A Sprint 006.2 integra:

```text
Ethernet Header
+ GOOSE Protocol Header (8 bytes)
+ GOOSE APDU (0x61)
```

## Estrutura sem VLAN

```text
6 bytes  MAC destino
6 bytes  MAC origem
2 bytes  EtherType 0x88B8
2 bytes  APPID
2 bytes  Length
2 bytes  Reserved 1
2 bytes  Reserved 2
N bytes  GOOSE APDU
```

O valor `Length` inclui os oito bytes do cabeçalho GOOSE e toda a APDU.
O FCS não é incluído: o controlador Ethernet deve gerá-lo.

## Memória

Todos os buffers são fornecidos pelo chamador. Não há `malloc`, `new` ou
alocação dinâmica oculta.

## Limitação

Esta versão gera quadro sem VLAN. O suporte IEEE 802.1Q será integrado em uma
Sprint separada.
