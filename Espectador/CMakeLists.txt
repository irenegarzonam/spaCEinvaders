cmake_minimum_required(VERSION 3.25)
project(Espectador C)

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_C_STANDARD 11)
link_libraries(ws2_32 wsock32)

add_executable(Espectador Espectador.c Espectador.h)
target_include_directories(Espectador PRIVATE ${PROJECT_DIR})
file(COPY ${PROJECT_DIR}/imagenes DESTINATION ${CMAKE_CURRENT_BINARY_DIR})