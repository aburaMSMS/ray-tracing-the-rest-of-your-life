cmake_minimum_required(VERSION 3.0)

project(RayTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

macro(FIND_INCLUDE_DIR result curdir)
    file(GLOB_RECURSE children "${curdir}/*.hpp" "${curdir}/*.h")
    set(dirlist "")

    foreach(child ${children})
        string(REGEX REPLACE "(.*)/.*" "\\1" LIB_NAME ${child})

        if(IS_DIRECTORY ${LIB_NAME})
            list(FIND dirlist ${LIB_NAME} list_index)

            if(${list_index} LESS 0)
                LIST(APPEND dirlist ${LIB_NAME})
            endif()
        endif()
    endforeach()

    set(${result} ${dirlist})
endmacro()

FIND_INCLUDE_DIR(INC_DIR_LIST "include")

include_directories(${INC_DIR_LIST} ./external)

add_executable(${PROJECT_NAME} ./main.cpp)

file(GLOB_RECURSE SRC_DIR_LIST "src/*.cpp")

target_sources(
    ${PROJECT_NAME} PUBLIC
    ${SRC_DIR_LIST}
)
