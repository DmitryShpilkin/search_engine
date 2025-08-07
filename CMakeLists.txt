cmake_minimum_required(VERSION 3.21)
project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Передаём макрос с путём к корню проекта
add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Добавляем подпроекты
add_subdirectory(src)
add_subdirectory(tests)
