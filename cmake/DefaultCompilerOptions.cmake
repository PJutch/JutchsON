macro(setDefaultCompilerOptions target type)
    if(MSVC)
      target_compile_options(${target} ${type} /W4 /external:anglebrackets /external:W0 /external:templates- /utf-8)
    else()
      target_compile_options(${target} ${type} -Wall -Wextra -Wpedantic)
    endif()
endmacro()
