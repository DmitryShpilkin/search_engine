include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(search_tests search_tests.cpp)
target_link_libraries(search_tests PRIVATE search_engine_lib gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(search_tests)
