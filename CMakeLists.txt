# This file specifies how the project should be built, using CMake.
# If you are unfamiliar with CMake, don't worry about all the details.
# The sections you might want to edit are marked as such, and 
# the comments should hopefully make most of it clear.
#
# For many purposes, you may not need to change anything about this file.

cmake_minimum_required(VERSION 3.1.3)

# Set project name, version and laguages here. (change as needed)
# Version numbers are available by including "exampleConfig.h" in 
# the source. See exampleConfig.h.in for some more details.
project(CPP_BOILERPLATE VERSION 1.2.3.4 LANGUAGES CXX)


# Include stuff. No change needed.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)
include(Colors)

add_definitions(-DDNN_USE_IMAGE_API)
add_definitions(-DCNN_USE_SSE)
add_definitions(-DCNN_USE_AVX)

# --------------------------------------------------------------------------------
#                         Locate files (change as needed).
# --------------------------------------------------------------------------------
set(SOURCES 
    src/example.cpp
)
set(TESTFILES
    tests/dummy.cpp
    tests/main.cpp
)

# --------------------------------------------------------------------------------
#                            Build! (Change as needed)
# --------------------------------------------------------------------------------
# Compile all sources into a library. Called engine here (change if you wish).
add_library(engine ${SOURCES})

# Engine needs its header files, and users of the library must also see these (PUBLIC). (No change needed)
target_include_directories(engine 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Add an executable for the file main.cpp, here called main.x.
# If you add more executables, copy these lines accordingly.
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(/home/pazzari/Documents/GitHub/tiny-dnn)

add_executable(main.x app/main.cpp)   # Name of exec. and location of file.
target_link_libraries(main.x PRIVATE engine Threads::Threads)  # Link the executable to `engine` (if it uses it).

add_executable(train_LeNet app/train_LeNet98.cpp)   # Name of exec. and location of file.
target_link_libraries(train_LeNet PRIVATE engine Threads::Threads)  # Link the executable to `engine` (if it uses it).

add_executable(test_LeNet app/test_LeNet98.cpp)   # Name of exec. and location of file.
target_link_libraries(test_LeNet PRIVATE engine Threads::Threads)  # Link the executable to `engine` (if it uses it).

# Set the compile options you want, possibly depending on compiler (change as needed).
# Do similar for the executables if you wish to set options for them as well.
target_compile_options(engine PRIVATE 
                           # clang/GCC warnings
                           $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: 
                           "-Wall" "-Wextra" >
                           # MSVC warnings
                           $<$<CXX_COMPILER_ID:MSVC>: 
                           "/Wall" >
)

# Unix
if(CMAKE_COMPILER_IS_GNUCXX OR MINGW OR
   CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     check_cxx_compiler_flag("-msse3" COMPILER_HAS_SSE_FLAG)
#     check_cxx_compiler_flag("-mavx"  COMPILER_HAS_AVX_FLAG)
#         
#     if(COMPILER_HAS_SSE_FLAG)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -msse3")
#     endif(COMPILER_HAS_SSE_FLAG)
#     if(COMPILER_HAS_AVX_FLAG)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -mavx")
#     endif(COMPILER_HAS_AVX_FLAG)
endif()
# Set the properties you require, e.g. what C++ standard to use (change as needed).
set_target_properties(engine PROPERTIES 
    CXX_STANDARD 14 
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO
)

set_target_properties(main.x train_LeNet test_LeNet PROPERTIES 
    CXX_STANDARD 14 
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS YES
)

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS}")

# --------------------------------------------------------------------------------
#                         Make Tests (no change needed).
# --------------------------------------------------------------------------------
set(TEST_MAIN unit_tests.x)  # Default name for test executable (change if you wish).

# Add a make target 'gtest', that runs the tests (and builds all dependencies).
# The setup of Google Test is done at the very end of this file.
add_executable(${TEST_MAIN} ${TESTFILES})
target_link_libraries(${TEST_MAIN} googletest engine pthread)
add_custom_target(gtest 
    COMMAND "${PROJECT_BINARY_DIR}/${TEST_MAIN}"
    DEPENDS ${TEST_MAIN})


# Add a standard make target 'test' that runs the tests under CTest (only as an alt. to gtest).
include(CTest)
enable_testing()
add_test(unit_tests ${PROJECT_BINARY_DIR}/${TEST_MAIN})

# --------------------------------------------------------------------------------
#                         Code Coverage (no change needed).
# --------------------------------------------------------------------------------
# If CMake aborts due to missing dependencies for code coverage 
# (gcov, lcov, genhtml and supported compiler), comment this line.
include(CodeCoverage)

# --------------------------------------------------------------------------------
#                         Documentation (no change needed).
# --------------------------------------------------------------------------------
# Add a make target 'doc' to generate API documentation with Doxygen.
# You should set options to your liking in the file 'Doxyfile.in'.
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc 
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile &> doxygen.log
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "${BoldMagenta}Generating API documentation with Doxygen (open ./html/index.html to view).${ColourReset}" VERBATIM
    )
endif(DOXYGEN_FOUND)


# --------------------------------------------------------------------------------
#                         Google Test (no change needed).
# --------------------------------------------------------------------------------
# The following makes sure that an up-to-date version of googletest is available,
# and built so that it may be used by your tests. 
add_custom_target(git_update
    COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_library( googletest
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest_main.cc
)
target_include_directories(googletest 
    PUBLIC ${PROJECT_SOURCE_DIR}/external/googletest/include 
           ${PROJECT_SOURCE_DIR}/external/googletest
)
add_dependencies(googletest git_update)
set_source_files_properties(
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest-all.cc  
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest_main.cc
    PROPERTIES GENERATED 1
)


# --------------------------------------------------------------------------------
#                            Misc (no change needed).
# --------------------------------------------------------------------------------
# Have CMake parse the config file, generating the config header, with
# correct definitions. Here only used to make version number available to
# the source code. Include "exampleConfig.h" (no .in suffix) in the source.
configure_file (
  "${PROJECT_SOURCE_DIR}/include/exampleConfig.h.in"
  "${PROJECT_BINARY_DIR}/exampleConfig.h"
)
# add the binary tree to the search path for include files
# so that we will find exampleConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# Ask CMake to output a compile_commands.json file for use with things like Vim YCM.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
