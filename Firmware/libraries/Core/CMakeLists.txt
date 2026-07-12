cmake_minimum_required(VERSION 3.16)
project(MGE-Core VERSION 0.3.9 LANGUAGES CXX)

add_library(mge-core STATIC
    src/buffer_reader.cpp
    src/buffer_writer.cpp
)

target_include_directories(mge-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(mge-core PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(mge-core PRIVATE /W4)
else()
    target_compile_options(mge-core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(buffer_reader_test tests/buffer_reader_test.cpp)
target_link_libraries(buffer_reader_test PRIVATE mge-core)
add_test(NAME buffer_reader_test COMMAND buffer_reader_test)

add_executable(buffer_writer_test tests/buffer_writer_test.cpp)
target_link_libraries(buffer_writer_test PRIVATE mge-core)
add_test(NAME buffer_writer_test COMMAND buffer_writer_test)
