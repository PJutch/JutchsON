enable_testing()

add_executable(tests "num.cpp" "Location.cpp" "ParseResult.cpp" "escape.cpp" "list.cpp")

target_link_libraries(tests test_dependencies)
target_include_directories(tests PRIVATE ../src)
setDefaultCompilerOptions(tests PRIVATE)

include(GoogleTest)
gtest_discover_tests(tests)
