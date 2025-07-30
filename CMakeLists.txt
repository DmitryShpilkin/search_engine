cmake_minimum_required(VERSION 3.21)
project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Передаём макрос с путём к корню проекта
add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Подключаем внешние зависимости (GoogleTest)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_disable_pthreads ON)
FetchContent_MakeAvailable(googletest)

# Добавляем подпроекты
add_subdirectory(src)
add_subdirectory(tests)
