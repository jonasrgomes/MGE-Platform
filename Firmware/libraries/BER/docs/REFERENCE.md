# BER Foundation Reference

## Referência normativa

- ITU-T X.690
- Basic Encoding Rules (BER)

## Escopo da Sprint 004.1

Implementado:

- classe de tag;
- forma curta de tag;
- forma longa de tag;
- comprimento curto;
- comprimento longo definido;
- encoder TLV genérico;
- decoder TLV zero-copy.

Ainda não implementado:

- comprimento indefinido;
- tipos BOOLEAN e INTEGER de alto nível;
- BIT STRING;
- OCTET STRING;
- sequências construídas completas.

## Formato TLV

```text
Tag | Length | Value
```

Exemplo:

```text
01 01 FF
```

- `01`: BOOLEAN;
- `01`: comprimento 1;
- `FF`: TRUE.
