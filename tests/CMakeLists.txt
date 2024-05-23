enable_testing()

add_executable(tests "num.cpp" "Location.cpp" "ParseResult.cpp")

target_link_libraries(tests test_dependencies)
target_include_directories(tests PRIVATE ../src)

include(GoogleTest)
gtest_discover_tests(tests)
