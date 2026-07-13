# Sprint 005.2 — ASN.1 Dataset

## Implementado

- `Dataset<Capacity>` com armazenamento estático;
- capacidade definida em tempo de compilação;
- adição de `NULL`, `BOOLEAN`, `INTEGER`, `OCTET STRING`,
  `VisibleString`, `UTF8String` e `BIT STRING`;
- acesso indexado;
- detecção de capacidade esgotada;
- `DatasetEncoder`;
- `DatasetDecoder`;
- codificação sequencial;
- decodificação zero-copy;
- teste de ida e volta;
- exemplo;
- documentação;
- CMake atualizado para `0.5.2`.

## Validação

O projeto completo foi compilado em C++17.

```text
11 de 11 testes aprovados
0 falhas
```

## Integração

Extraia este ZIP na raiz do repositório e aceite a substituição do
`Firmware/Libraries/ASN1/CMakeLists.txt`.

## Commit sugerido

```text
ASN1: adiciona Dataset estático e codecs sequenciais
```

## Próxima Sprint

Sprint 006.1 — GOOSE Foundation:

- cabeçalho GOOSE;
- APPID;
- Length;
- Reserved 1 e 2;
- `GooseMessage`;
- `GooseDataset`;
- estrutura inicial do APDU.
