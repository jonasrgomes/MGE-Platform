# MGE Ethernet

Biblioteca Ethernet portátil da MGE Platform.

## Versão atual

`0.1.0`

## Conteúdo

- `MacAddress`;
- `EtherType`;
- teste unitário;
- exemplo de uso;
- especificação Ethernet;
- projeto CMake.

## Compilação

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Próxima etapa

Implementação de `EthernetFrame` e `EthernetBuilder`.
