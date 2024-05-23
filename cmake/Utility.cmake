macro(fetchGit contentName file repository tag)
    message("Fetching ${contentName} (file ${file}) from ${repository} with tag ${tag}")
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../deps/${file}")
        FetchContent_Declare(
            ${contentName}
            URL ${CMAKE_CURRENT_LIST_DIR}/../deps/${file}
            DOWNLOAD_EXTRACT_TIMESTAMP true
        )
    else()
        FetchContent_Declare(
            ${contentName}
            GIT_REPOSITORY ${repository}
            GIT_TAG ${tag}
            DOWNLOAD_EXTRACT_TIMESTAMP true
        )
    endif()

    FetchContent_MakeAvailable(${contentName})
endmacro()

macro(fetchUrl contentName file url)
    message("Fetching ${contentName} (file ${file}) from ${url}")
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../deps/${file}")
        FetchContent_Declare(
            ${contentName}
            URL ${CMAKE_CURRENT_LIST_DIR}/../deps/${file}
            DOWNLOAD_EXTRACT_TIMESTAMP true
        )
    else()
        FetchContent_Declare(
            ${contentName}
            URL ${url}
            DOWNLOAD_EXTRACT_TIMESTAMP true
        )
    endif()

    FetchContent_MakeAvailable(${contentName})
endmacro()

macro(addDependency dependencyLibrary dependency)
    if(NOT TARGET ${dependency})
        message(FATAL_ERROR "Dependency ${dependency} is not satisfied")
    endif()

    if(NOT TARGET ${dependencyLibrary})
        add_library(${dependencyLibrary} INTERFACE)
    endif()

    target_link_libraries(${dependencyLibrary} INTERFACE ${dependency})
endmacro()
