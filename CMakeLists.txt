cmake_minimum_required(VERSION 3.20.1)
project(SimplePool VERSION 0.1.0 LANGUAGES CXX)

option(EXPORT_SHARED "Build SimplePool as a shared library" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(SIMPLE_POOL_EXPORT_TARGET     "${PROJECT_NAME}Targets")

if(EXPORT_SHARED)
    add_library(${PROJECT_NAME} SHARED)
else() 
    add_library(${PROJECT_NAME} STATIC)
endif()

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

target_sources(${PROJECT_NAME} PRIVATE
    ${SRC}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_LIB)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES 
                CXX_VISIBILITY_PRESET hidden
                C_VISIBILITY_PRESET hidden
                VISIBILITY_INLINES_HIDDEN ON    
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/simple_pool.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include)


# install config
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${SIMPLE_POOL_EXPORT_TARGET}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${SIMPLE_POOL_EXPORT_TARGET}
    FILE "${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}
)