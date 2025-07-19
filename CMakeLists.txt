# Минимальная версия CMake, необходимая для проекта
cmake_minimum_required(VERSION 3.21)

# Имя проекта и язык
project(search_engine LANGUAGES CXX)

# Передаём путь к корню проекта как макрос PROJECT_ROOT_DIR
add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление директорий с заголовками
include_directories(include)
include_directories(third_party/nlohmann_json/include)

# ===================== GoogleTest =====================

# Встроенная система загрузки внешних зависимостей
include(FetchContent)

# Загрузка GoogleTest из официального архива
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

# Отключение потоков GTest для MinGW
set(gtest_disable_pthreads ON)

# Разрешить использование общей CRT библиотеки (важно для Windows)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Скачивание и сборка GoogleTest
FetchContent_MakeAvailable(googletest)

# Включаем поддержку тестов в CMake
enable_testing()

# ===================== Сборка библиотеки проекта =====================

# Создаём статическую библиотеку из основных исходников
add_library(search_engine_lib
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
)

# Указываем, где искать заголовочные файлы для этой библиотеки
target_include_directories(search_engine_lib PUBLIC include)

# ===================== Сборка основного исполняемого файла =====================

# Создаём исполняемый файл `search_engine` из main.cpp
add_executable(search_engine src/main.cpp)

# Линкуем основной исполняемый файл с нашей библиотекой
target_link_libraries(search_engine PRIVATE search_engine_lib)

# ===================== Сборка тестов =====================

# Создаём исполняемый файл для тестов
add_executable(search_tests tests/search_tests.cpp)

# Линкуем тесты с нашей библиотекой и GoogleTest
target_link_libraries(search_tests PRIVATE search_engine_lib gtest_main)

# Регистрируем тесты для запуска через CTest
include(GoogleTest)
gtest_discover_tests(search_tests)

# ===================== Работа с ресурсами =====================

# Устанавливаем путь к папке с ресурсами (JSON-файлы и т. п.)
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(OUTPUT_RESOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

# После сборки копируем папку ресурсов в директорию билда
add_custom_command(
        TARGET search_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCES_DIR}
        ${OUTPUT_RESOURCES_DIR}
)

# Передаём путь к папке ресурсов в код через макрос RESOURCES_DIR
target_compile_definitions(search_engine_lib PUBLIC RESOURCES_DIR="${OUTPUT_RESOURCES_DIR}")
target_compile_definitions(search_engine PRIVATE RESOURCES_DIR="${OUTPUT_RESOURCES_DIR}")
