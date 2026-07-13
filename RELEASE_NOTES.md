# Sprint 005.1 — ASN.1 Foundation

## Implementado

- `asn1::Type`
- `asn1::Value`
- `asn1::Encoder`
- `asn1::Decoder`
- NULL
- BOOLEAN
- INTEGER
- OCTET STRING
- VisibleString
- UTF8String
- BIT STRING
- armazenamento zero-copy com `BufferView`
- testes de ida e volta
- exemplo
- integração com CMake

## Validação

Projeto completo compilado em C++17.

```text
10 de 10 testes aprovados
0 falhas
```

## Integração

Extraia o ZIP na raiz do repositório e aceite a substituição do
`CMakeLists.txt` principal.

## Commit sugerido

```text
ASN1: adiciona foundation de tipos, Value, Encoder e Decoder
```

## Próxima Sprint

Sprint 005.2:

- coleção estática de valores;
- dataset ASN.1;
- limite configurável de entradas;
- iteração sem alocação dinâmica;
- base para o `allData` do GOOSE.
