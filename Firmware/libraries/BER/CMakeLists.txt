cmake_minimum_required(VERSION 3.16)

project(MGE-BER VERSION 0.4.1 LANGUAGES CXX)

add_library(mge-ber STATIC
    src/ber_length.cpp
    src/ber_encoder.cpp
    src/ber_decoder.cpp
)

target_include_directories(mge-ber
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/include
)

target_compile_features(mge-ber PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(mge-ber PRIVATE /W4)
else()
    target_compile_options(
        mge-ber
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

add_executable(ber_foundation_test
    tests/ber_foundation_test.cpp
)

target_link_libraries(ber_foundation_test
    PRIVATE
        mge-ber
)

add_executable(ber_foundation_example
    examples/ber_foundation_example.cpp
)

target_link_libraries(ber_foundation_example
    PRIVATE
        mge-ber
)

enable_testing()

add_test(
    NAME ber_foundation_test
    COMMAND ber_foundation_test
)
