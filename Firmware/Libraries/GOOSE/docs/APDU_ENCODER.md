# GOOSE APDU Encoder — Sprint 006.1B

Codifica a APDU GOOSE com a tag externa `0x61` e os campos context-specific:

| Campo | Tag |
|---|---:|
| gocbRef | 0x80 |
| timeAllowedToLive | 0x81 |
| datSet | 0x82 |
| goID | 0x83 |
| t | 0x84 |
| stNum | 0x85 |
| sqNum | 0x86 |
| test | 0x87 |
| confRev | 0x88 |
| ndsCom | 0x89 |
| numDatSetEntries | 0x8A |
| allData | 0xAB |

## Memória

O chamador fornece dois buffers de trabalho. Isso evita `malloc`, `new` e
arrays grandes ocultos na pilha do ESP32.

## allData

Os valores usam as tags IEC 61850/MMS Data, e não as tags universais ASN.1:

- BOOLEAN: `[3]` / `0x83`;
- BIT STRING: `[4]` / `0x84`;
- INTEGER: `[5]` / `0x85`;
- OCTET STRING: `[9]` / `0x89`;
- VisibleString: `[10]` / `0x8A`;
- mMSString/UTF8: `[16]`.
