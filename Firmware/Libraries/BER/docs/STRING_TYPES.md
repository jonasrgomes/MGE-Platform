# BER String Types — Sprint 004.3

Implementados:

- OCTET STRING, tag `0x04`;
- UTF8String, tag `0x0C`;
- VisibleString, tag `0x1A`;
- BIT STRING, tag `0x03`.

## BIT STRING

O primeiro byte do valor indica quantos bits não utilizados existem no último byte.

Exemplo para os bits `1011`:

```text
03 02 04 B0
```

- `03`: BIT STRING;
- `02`: comprimento do valor;
- `04`: quatro bits não utilizados;
- `B0`: `10110000`.

Os decoders retornam visualizações zero-copy sobre os dados codificados.
