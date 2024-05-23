cmake_minimum_required(VERSION 3.20)

cmake_policy(SET CMP0048 NEW) 
project(JutchsON VERSION 0.1
        DESCRIPTION "Custom human-readable data format"
        LANGUAGES CXX)
        
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:preprocessor")
endif()

include(cmake/FetchDependencies.cmake)

include(cmake/DefaultCompilerOptions.cmake)

add_library(JutchsON INTERFACE src/JutchsON.hpp)
target_link_libraries(JutchsON INTERFACE sources dependencies)
setDefaultCompilerOptions(JutchsON)

add_subdirectory(tests)

# add_custom_target(doc COMMAND doxygen doc/doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
