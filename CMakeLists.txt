cmake_minimum_required(VERSION 3.2)
project(cppcli LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(cppcli ${SOURCES})
target_link_libraries(cppcli PRIVATE boost_program_options)
target_compile_features(cppcli PRIVATE cxx_std_23)
add_compile_options(-Wall -Wextra)

set(CMAKE_INSTALL_PREFIX /usr)

install(TARGETS cppcli)
