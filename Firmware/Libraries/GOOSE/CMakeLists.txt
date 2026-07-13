cmake_minimum_required(VERSION 3.16)
project(MGE-GOOSE VERSION 0.6.1 LANGUAGES CXX)

add_library(mge-goose STATIC
    src/goose_header.cpp
    src/goose_control_block.cpp
)

target_include_directories(mge-goose PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../Core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../Ethernet/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../BER/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../ASN1/include
)

target_link_libraries(mge-goose PUBLIC
    mge-core
    mge-ethernet
    mge-ber
    mge-asn1
)

target_compile_features(mge-goose PUBLIC cxx_std_17)

add_executable(goose_foundation_test tests/goose_foundation_test.cpp)
target_link_libraries(goose_foundation_test PRIVATE mge-goose)

add_executable(goose_foundation_example examples/goose_foundation_example.cpp)
target_link_libraries(goose_foundation_example PRIVATE mge-goose)

enable_testing()
add_test(NAME goose_foundation_test COMMAND goose_foundation_test)
