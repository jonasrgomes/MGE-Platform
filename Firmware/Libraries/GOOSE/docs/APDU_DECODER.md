# GOOSE APDU Decoder

Decodifica a APDU IEC 61850 GOOSE com tag externa `0x61`.

## Campos suportados

- `gocbRef` `[0]`
- `timeAllowedToLive` `[1]`
- `datSet` `[2]`
- `goID` `[3]`
- `t` `[4]`
- `stNum` `[5]`
- `sqNum` `[6]`
- `test` `[7]`
- `confRev` `[8]`
- `ndsCom` `[9]`
- `numDatSetEntries` `[10]`
- `allData` `[11]`

## Zero-copy

O conteúdo de OCTET STRING, VisibleString, UTF8String e BIT STRING em
`allData` continua apontando para o buffer recebido. O chamador deve manter o
buffer da APDU válido enquanto utilizar o `Message` decodificado.

## Validações

- tag externa Application 1;
- todos os campos obrigatórios;
- campos duplicados;
- comprimentos;
- número declarado de entradas;
- capacidade do dataset;
- tags de dados suportadas.
