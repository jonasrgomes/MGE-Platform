# BER Primitive Types

## BOOLEAN

Codificação:

```text
01 01 00    FALSE
01 01 FF    TRUE
```

O decoder aceita qualquer valor diferente de zero como verdadeiro.

## INTEGER

O INTEGER BER utiliza complemento de dois e o menor número de bytes capaz de
preservar o sinal.

Exemplos:

```text
02 01 7F       127
02 02 00 80    128
02 01 FF       -1
02 02 FF 7F    -129
```

## ENUMERATED

Utiliza a mesma representação de INTEGER, com tag universal `0x0A`.

Exemplo:

```text
0A 01 03
```

## NULL

Sempre possui comprimento zero:

```text
05 00
```
