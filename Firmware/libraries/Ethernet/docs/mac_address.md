# MacAddress

Representa um endereço MAC Ethernet de 48 bits.

## Exemplo

```cpp
MacAddress destination(
    0x01, 0x0C, 0xCD, 0x01, 0x00, 0x01
);
```

## Características

- sem alocação dinâmica;
- comparação;
- detecção de broadcast;
- detecção de multicast;
- conversão para texto;
- acesso aos seis bytes.
