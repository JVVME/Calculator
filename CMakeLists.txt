cmake_minimum_required(VERSION 3.30)
project(Calculator)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_executable(Calculator main.cpp
        kernel.cpp
        include/kernel.h
        Log.cpp
        include/Log.h
        help.cpp
        include/help.h
        include/Information.h
        LAlgebra.cpp
        include/LAlgebra.h
)
