cmake_minimum_required(VERSION 3.16)
project(MGE-ASN1 VERSION 0.5.2 LANGUAGES CXX)

add_library(mge-asn1 STATIC
    src/asn1_encoder.cpp
    src/asn1_decoder.cpp
)

target_include_directories(mge-asn1 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../Core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../BER/include
)

target_link_libraries(mge-asn1 PUBLIC mge-ber)
target_compile_features(mge-asn1 PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(mge-asn1 PRIVATE /W4)
else()
    target_compile_options(mge-asn1 PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(asn1_foundation_test tests/asn1_foundation_test.cpp)
target_link_libraries(asn1_foundation_test PRIVATE mge-asn1)

add_executable(asn1_foundation_example examples/asn1_foundation_example.cpp)
target_link_libraries(asn1_foundation_example PRIVATE mge-asn1)

enable_testing()
add_test(NAME asn1_foundation_test COMMAND asn1_foundation_test)

add_executable(asn1_dataset_test tests/asn1_dataset_test.cpp)
target_link_libraries(asn1_dataset_test PRIVATE mge-asn1)

add_executable(asn1_dataset_example examples/asn1_dataset_example.cpp)
target_link_libraries(asn1_dataset_example PRIVATE mge-asn1)

add_test(NAME asn1_dataset_test COMMAND asn1_dataset_test)
