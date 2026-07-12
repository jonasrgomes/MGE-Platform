# MGE Platform — Arquitetura do Sistema

**Versão:** 0.1.0  
**Autor:** Jonas Rodrigues Gomes  
**Status:** Em desenvolvimento

---

## 1. Objetivo

A MGE Platform é uma plataforma modular de automação industrial destinada ao desenvolvimento de:

- IEDs;
- remotas de entradas e saídas;
- gateways industriais;
- simuladores;
- ferramentas de teste;
- sistemas de comunicação IEC 61850;
- controladores programáveis.

O primeiro hardware suportado será:

- ESP32-WROOM;
- Ethernet W5500 em modo MACRAW;
- expansor MCP23017;
- 6 entradas digitais;
- 10 saídas digitais;
- comunicação GOOSE com RTAC.

---

## 2. Princípios do projeto

A plataforma deve seguir os seguintes princípios:

1. Código C++17.
2. Arquitetura modular.
3. Independência de hardware.
4. Ausência de alocação dinâmica em tempo de execução.
5. Inicialização segura das saídas.
6. Testes em Windows e Linux antes da integração no ESP32.
7. Separação entre bibliotecas, drivers e aplicações.
8. Tratamento de erros padronizado.
9. Documentação de todas as interfaces públicas.
10. Compatibilidade futura com diferentes microcontroladores.

---

## 3. Restrições

Não utilizar no núcleo embarcado:

```text
malloc
calloc
realloc
free
new
delete
String do Arduino
exceções C++
RTTI