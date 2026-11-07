cmake_minimum_required(VERSION 3.20.1)
project(SimplePool VERSION 1.0.0 LANGUAGES CXX)

option(EXPORT_SHARED "Build SimplePool as a shared library" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 17)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/simple_pool.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include)
if(EXPORT_SHARED)
    add_library(SimplePool SHARED ${SRC})
else() 
    add_library(SimplePool STATIC ${SRC})
endif()

target_compile_definitions(SimplePool PRIVATE BUILD_LIB)

set_target_properties(
    SimplePool PROPERTIES 
                PREFIX ""
                CXX_VISIBILITY_PRESET hidden
                C_VISIBILITY_PRESET hidden
                VISIBILITY_INLINES_HIDDEN ON    
)
