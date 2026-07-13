# GOOSE FrameDecoder

O `FrameDecoder` interpreta um quadro Ethernet GOOSE completo sem VLAN e sem
FCS.

## Fluxo

```text
Quadro Ethernet
    ↓
EthernetParser
    ↓
EtherType 0x88B8
    ↓
Header GOOSE de 8 bytes
    ↓
APPID / Length / Reserved
    ↓
ApduDecoder
    ↓
Message + Dataset
```

## Campo Length

O campo `Length` inclui:

```text
8 bytes do cabeçalho GOOSE + tamanho da APDU
```

O decoder usa esse valor para separar a mensagem real dos bytes de padding do
quadro Ethernet.

## Zero-copy

Strings, OCTET STRING e BIT STRING do dataset decodificado continuam apontando
para o quadro Ethernet original. O buffer recebido deve permanecer válido
enquanto a mensagem for utilizada.

## Limitações

Esta versão aceita quadros Ethernet GOOSE sem VLAN. O suporte IEEE 802.1Q será
adicionado em uma Sprint específica.
