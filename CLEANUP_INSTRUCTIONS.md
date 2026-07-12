# Sprint 003.9 — Architecture Cleanup

Este pacote é uma cópia completa e limpa do repositório, sem `.git` e sem as pastas duplicadas.

## Forma segura de aplicar

1. Feche o GitHub Desktop.
2. Renomeie sua pasta atual `MGE-Platform` para `MGE-Platform-backup`.
3. Extraia este pacote.
4. Copie a pasta oculta `.git` do backup para a nova pasta **somente se necessário**; a forma preferida é copiar o conteúdo limpo sobre o clone atual após apagar as pastas duplicadas.
5. Abra o GitHub Desktop e confira as alterações.

## Pastas antigas que devem ser removidas do clone atual

- `Firmware/Core`, `Firmware/BER`, `Firmware/ASN1`, `Firmware/GOOSE`, `Firmware/IED`, `Firmware/Logic`
- `Firmware/MGE-*`
- `Firmware/libraries` (minúsculo), depois substituída por `Firmware/Libraries`

## Estrutura oficial

- `Firmware/Libraries`
- `Firmware/Drivers`
- `Firmware/Applications`
