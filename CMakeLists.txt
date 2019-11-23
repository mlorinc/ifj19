cmake_minimum_required(VERSION 3.10)
project(IFJ19 VERSION 1.0 LANGUAGES CXX C)


# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)

    # Docs only available if this is the main app
    
endif()

# The compiled library code is here
add_subdirectory(src)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    option(PACKAGE_TESTS "Build the tests" ON)

    if(PACKAGE_TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()

if(CMAKE_VERSION VERSION_GREATER 3.2.0)
    add_custom_target(pack COMMAND
        ${CMAKE_COMMAND} -E tar "cfv" "xlogin00.zip" --format=zip
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Makefile")
endif()
