cmake_minimum_required(VERSION 3.10)

# Название проекта
project(MarioClone)

# Установка версии C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Установка компилятора Clang
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# Укажите пути вручную
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories("/opt/homebrew/Cellar/sfml/3.0.0_1/lib")

# Укажите путь к SFML
set(SFML_INCLUDE_DIR "/opt/homebrew/Cellar/sfml/3.0.0_1/include")
include_directories(${SFML_INCLUDE_DIR})

# Добавление исходников
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Создание исполняемого файла
add_executable(main ${SOURCES})

# Подключение библиотек SFML
target_link_libraries(main sfml-graphics sfml-window sfml-system)

# Настройка директории для вывода
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Цель для выполнения ./main
add_custom_target(
    run
    COMMAND ./main
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    DEPENDS main
)
