# Заголовки и сторонние библиотеки
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/nlohmann_json/include)

# Библиотека
add_library(search_engine_lib
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
)

target_include_directories(search_engine_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Исполняемый файл
add_executable(search_engine main.cpp)
target_link_libraries(search_engine PRIVATE search_engine_lib)

# Работа с ресурсами
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(OUTPUT_RESOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
        TARGET search_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCES_DIR}
        ${OUTPUT_RESOURCES_DIR}
)

# Передаём путь к ресурсам в код
target_compile_definitions(search_engine_lib PUBLIC RESOURCES_DIR="${OUTPUT_RESOURCES_DIR}")
target_compile_definitions(search_engine PRIVATE RESOURCES_DIR="${OUTPUT_RESOURCES_DIR}")
