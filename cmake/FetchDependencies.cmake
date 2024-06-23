add_library(JutchsON_dependencies INTERFACE)
add_library(JutchsON_test_dependencies INTERFACE)

include(FetchContent)

include(cmake/FetchBoost.cmake)

if(NOT TARGET GTest::gtest_main)
    include(cmake/Utility.cmake)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    fetchGit(googletest googletest.zip https://github.com/google/googletest origin/v1.13.x)
endif()

addDependency(JutchsON_test_dependencies GTest::gtest_main)

target_link_libraries(JutchsON_test_dependencies INTERFACE JutchsON_dependencies)
