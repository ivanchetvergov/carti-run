cmake_minimum_required(VERSION 3.10)

project(MarioClone)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# укажи компилятор
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# пути к sfml
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories("/opt/homebrew/Cellar/sfml/3.0.0_1/lib")

set(SFML_INCLUDE_DIR "/opt/homebrew/Cellar/sfml/3.0.0_1/include")
include_directories(${SFML_INCLUDE_DIR})

# исходники
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# исполняемый файл
add_executable(main ${SOURCES})

# подключение sfml
target_link_libraries(main sfml-graphics sfml-window sfml-system sfml-audio)

# вывод бинарников
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# обычный запуск
add_custom_target(
    run
    COMMAND ./main
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    DEPENDS main
)

# debug-режим
set(CMAKE_CXX_FLAGS_DEBUG "-g")  # включаем отладочную информацию

add_custom_target(
    debug
    COMMAND gdb ./main
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    DEPENDS main
)
