cmake_minimum_required(VERSION 3.16)
project(MGE-GOOSE VERSION 0.6.3 LANGUAGES CXX)

add_library(mge-goose STATIC
    src/goose_header.cpp
    src/goose_control_block.cpp
    src/goose_apdu_encoder.cpp
    src/goose_apdu_decoder.cpp
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

add_executable(goose_apdu_encoder_test tests/goose_apdu_encoder_test.cpp)
target_link_libraries(goose_apdu_encoder_test PRIVATE mge-goose)
add_test(NAME goose_apdu_encoder_test COMMAND goose_apdu_encoder_test)

add_executable(goose_apdu_encoder_example examples/goose_apdu_encoder_example.cpp)
target_link_libraries(goose_apdu_encoder_example PRIVATE mge-goose)

add_executable(goose_apdu_decoder_test tests/goose_apdu_decoder_test.cpp)
target_link_libraries(goose_apdu_decoder_test PRIVATE mge-goose)
add_test(NAME goose_apdu_decoder_test COMMAND goose_apdu_decoder_test)

add_executable(goose_apdu_decoder_example examples/goose_apdu_decoder_example.cpp)
target_link_libraries(goose_apdu_decoder_example PRIVATE mge-goose)

add_executable(goose_frame_encoder_test tests/goose_frame_encoder_test.cpp)
target_link_libraries(goose_frame_encoder_test PRIVATE mge-goose)
add_test(NAME goose_frame_encoder_test COMMAND goose_frame_encoder_test)

add_executable(goose_frame_encoder_example examples/goose_frame_encoder_example.cpp)
target_link_libraries(goose_frame_encoder_example PRIVATE mge-goose)

add_executable(goose_frame_decoder_test tests/goose_frame_decoder_test.cpp)
target_link_libraries(goose_frame_decoder_test PRIVATE mge-goose)
add_test(NAME goose_frame_decoder_test COMMAND goose_frame_decoder_test)

add_executable(goose_frame_decoder_example examples/goose_frame_decoder_example.cpp)
target_link_libraries(goose_frame_decoder_example PRIVATE mge-goose)
