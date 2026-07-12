# MGE Platform

Estrutura consolidada da plataforma MGE.

## Bibliotecas ativas

- Core
- Ethernet
- BER

## Compilar

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
