# Integração da Sprint 004.2

Este pacote substitui:

```text
include/mge/ber/ber_tag.h
include/mge/ber/ber_encoder.h
include/mge/ber/ber_decoder.h
src/ber_encoder.cpp
src/ber_decoder.cpp
```

E adiciona:

```text
tests/ber_primitive_types_test.cpp
examples/ber_primitive_types_example.cpp
docs/PRIMITIVE_TYPES.md
```

## CMake

Adicione:

```cmake
add_executable(ber_primitive_types_test
    tests/ber_primitive_types_test.cpp
)

target_link_libraries(ber_primitive_types_test
    PRIVATE
        mge-ber
)

add_test(
    NAME ber_primitive_types_test
    COMMAND ber_primitive_types_test
)

add_executable(ber_primitive_types_example
    examples/ber_primitive_types_example.cpp
)

target_link_libraries(ber_primitive_types_example
    PRIVATE
        mge-ber
)
```
