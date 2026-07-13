# BER Constructed Types and OBJECT IDENTIFIER

Implementado nesta Sprint:

- `SEQUENCE` e `SET` a partir de conteúdo BER já codificado;
- tags `ContextSpecific` e `Application`;
- `OBJECT IDENTIFIER` sem alocação dinâmica;
- decodificação zero-copy de conteúdo construído.

## Exemplo de SEQUENCE

```text
30 LL <conteúdo TLV>
```

## Exemplo de tag context-specific

```text
83 02 AA BB
```

## OBJECT IDENTIFIER

Os dois primeiros arcos são combinados como `40 * arc0 + arc1`. Cada
subidentificador é codificado em base 128, com o bit 8 indicando continuação.
