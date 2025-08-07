include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(search_tests search_tests.cpp)
target_link_libraries(search_tests PRIVATE search_engine_lib gtest_main)

# Подключаем внешние зависимости (GoogleTest)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_disable_pthreads ON)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)
gtest_discover_tests(search_tests)
