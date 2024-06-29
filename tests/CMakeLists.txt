enable_testing()

add_executable(JutchsON_tests "Location.cpp" "ParseResult.cpp" "escape.cpp"  "StringView.cpp" "object.cpp"   "strip.cpp"  
                              "parse/num.cpp" "write/num.cpp" "parse/list.cpp" "write/list.cpp" "parse/dict.cpp" "write/dict.cpp" 
                              "parse/bool.cpp" "write/bool.cpp" "parse/str.cpp" "write/str.cpp"  
                              "parse/tuple.cpp" "parse/struct.cpp" "write/tuple.cpp" "write/struct.cpp" 
                              "parse/variant.cpp" "write/variant.cpp" "parse/optional.cpp" "write/optional.cpp" "parse/type.cpp")

target_link_libraries(JutchsON_tests JutchsON JutchsON_test_dependencies)
setDefaultCompilerOptions(JutchsON_tests PRIVATE)

include(GoogleTest)
gtest_discover_tests(JutchsON_tests)
