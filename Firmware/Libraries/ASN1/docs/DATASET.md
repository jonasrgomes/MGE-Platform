# ASN.1 Dataset

`Dataset<Capacity>` armazena uma coleção estática de valores ASN.1.

## Características

- sem `malloc`, `new` ou `delete`;
- capacidade definida em tempo de compilação;
- valores armazenados em ordem;
- strings e octetos permanecem zero-copy por meio de `BufferView`;
- encoder sequencial;
- decoder com quantidade explícita de entradas;
- base para o campo `allData` do GOOSE.

## Exemplo

```cpp
asn1::Dataset<16> dataset;
dataset.addBoolean(true);
dataset.addInteger(25);
dataset.addVisibleString(nameView);
```

## Vida útil da memória

Valores baseados em `BufferView` não possuem a memória apontada. O chamador deve
manter os buffers válidos durante o uso do dataset.
