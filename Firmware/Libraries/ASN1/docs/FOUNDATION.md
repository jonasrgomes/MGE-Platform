# ASN.1 Foundation

## Objetivo

Fornecer uma representação estática de valores ASN.1 sobre a biblioteca BER.

## Tipos desta Sprint

- NULL
- BOOLEAN
- INTEGER
- OCTET STRING
- VisibleString
- UTF8String
- BIT STRING

## Zero-copy

Strings, octetos e bit strings utilizam `BufferView`. A classe `Value` não
possui nem libera a memória apontada.

## Limites atuais

- Não há alocação dinâmica.
- Não há árvore ASN.1 genérica.
- SEQUENCE e coleções serão adicionadas na próxima Sprint.
- Tipos MMS usados no GOOSE serão mapeados em uma camada própria.
