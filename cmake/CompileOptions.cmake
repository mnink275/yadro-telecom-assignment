set(CompilerFlags "")

if (MSVC)
    set(CompilerFlags /W4)  # Для MSVC вместо -Wextra используем /W4
else()
    set(CompilerFlags -Wall -Wextra -Wpedantic)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    if (MSVC)
        list(APPEND CompilerFlags /Od /DEBUG)  # Отключаем оптимизации в Debug
    else()
        list(APPEND CompilerFlags -g -O0)
    endif()
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    if (MSVC)
        list(APPEND CompilerFlags /O2)
    else()
        list(APPEND CompilerFlags -O2)
    endif()
endif()

message(STATUS "Target compiler flags are: ${CompilerFlags}")
add_compile_options(${CompilerFlags})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
