enable_testing()

add_executable(tests  "Location.cpp" "ParseResult.cpp" "escape.cpp"  "StringView.cpp" "object.cpp"  "str.cpp" "strip.cpp" "bool.cpp" "parse/num.cpp" "write/num.cpp" "parse/list.cpp" "write/list.cpp" "parse/dict.cpp" "write/dict.cpp")

target_link_libraries(tests test_dependencies)
target_include_directories(tests PRIVATE ../src)
setDefaultCompilerOptions(tests PRIVATE)

include(GoogleTest)
gtest_discover_tests(tests)
