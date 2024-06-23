include(cmake/Utility.cmake)

macro(fetchBoost module)
    fetchUrl(Boost.${module} 
             boost_${module}.zip 
             https://github.com/boostorg/${module}/archive/refs/tags/boost-1.82.0.zip)
endmacro()

if((NOT TARGET Boost.Stacktrace_windbg AND NOT TARGET boost_stacktrace_basic) OR NOT TARGET boost_describe)
    fetchBoost(throw_exception)
    fetchBoost(predef)
    fetchBoost(core)
    fetchBoost(config)
    fetchBoost(winapi)
    fetchBoost(static_assert)
    fetchBoost(assert)
    fetchBoost(array)
    fetchBoost(type_traits)
    fetchBoost(mp11)
    fetchBoost(container_hash)

    fetchBoost(describe)
    if(NOT TARGET boost_describe AND NOT TARGET boost_describe)
        message(FATAL_ERROR "Failed FetchContent: Boost.Describe")
    endif()

    fetchBoost(stacktrace)
    if(NOT TARGET boost_stacktrace_windbg AND NOT TARGET boost_stacktrace_basic)
        message(FATAL_ERROR "Failed FetchContent: Boost.Stacktrace")
    endif()
endif()

addDependency(JutchsON_dependencies boost_describe)

if(TARGET boost_stacktrace_windbg)
    addDependency(JutchsON_dependencies boost_stacktrace_windbg)
else()
    addDependency(JutchsON_dependencies boost_stacktrace_basic)
endif()
