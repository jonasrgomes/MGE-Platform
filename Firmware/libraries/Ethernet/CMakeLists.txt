cmake_minimum_required(VERSION 3.16)

project(MGE-Ethernet VERSION 0.1.0 LANGUAGES CXX)

add_library(mge-ethernet STATIC
    src/mac_address.cpp
)

target_include_directories(mge-ethernet
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(mge-ethernet PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(mge-ethernet PRIVATE /W4)
else()
    target_compile_options(mge-ethernet PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(mac_address_test
    tests/mac_address_test.cpp
)

target_link_libraries(mac_address_test
    PRIVATE
        mge-ethernet
)

add_executable(mac_address_example
    examples/mac_address_example.cpp
)

target_link_libraries(mac_address_example
    PRIVATE
        mge-ethernet
)

enable_testing()
add_test(NAME mac_address_test COMMAND mac_address_test)
