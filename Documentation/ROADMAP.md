# MGE Platform Coding Standard

Version: 0.1.0

---

# Objetivo

Garantir que todo o código da MGE Platform possua o mesmo padrão de qualidade,
facilitando manutenção, revisão e contribuição de novos desenvolvedores.

---

# Linguagem

- C++17
- Código portátil
- Compatível com GCC, Clang e MSVC

---

# Convenções de nomes

## Classes

```cpp
class Buffer;
class GoosePublisher;
class EthernetFrame;
```

PascalCase.

---

## Métodos

```cpp
writeU16();

readU32();

setEndian();
```

camelCase.

---

## Variáveis privadas

Sempre iniciar por:

```cpp
m_
```

Exemplo:

```cpp
m_buffer

m_capacity

m_position
```

---

## Constantes

```cpp
constexpr uint32_t DefaultBufferSize = 1500;
```

PascalCase.

---

## Enum class

```cpp
enum class Error
```

Nunca utilizar enum comum.

---

# Arquivos

Um arquivo deve possuir apenas uma responsabilidade.

Evitar arquivos maiores que 400 linhas.

---

# Includes

Sempre nesta ordem:

1. Arquivo correspondente

2. Bibliotecas C++

3. Bibliotecas da plataforma

4. Bibliotecas externas

---

# Namespace

Sempre:

```cpp
namespace mge
{
namespace core
{

}
}
```

---

# Comentários

Utilizar Doxygen.

```cpp
/**
 * @brief Escreve um inteiro de 16 bits.
 *
 * @param value Valor.
 *
 * @return Result
 */
```

---

# Ponteiros

Sempre inicializados.

```cpp
uint8_t* m_buffer = nullptr;
```

---

# Memória

Evitar:

- malloc
- calloc
- realloc
- free
- new
- delete

O firmware deverá utilizar buffers estáticos.

---

# Tratamento de erros

Nunca lançar exceções.

Sempre retornar:

```cpp
Result
```

---

# Testes

Toda biblioteca deverá possuir:

tests/

examples/

docs/

---

# Commits

Um commit = uma funcionalidade.

Exemplos:

Core: add Buffer

BER: add Integer encoder

GOOSE: add Publisher

---

# Pull Requests

Uma Pull Request deverá:

- compilar;
- possuir testes;
- atualizar documentação;
- não quebrar APIs públicas.

---

# Filosofia

Código simples.

Código seguro.

Código reutilizável.

Código documentado.

Código testado.