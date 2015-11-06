cmake_minimum_required (VERSION 2.6)
project (Compiler)

set(G_TEST_SOURCE_DIR "external_libs/googletest/googletest/include")
set(G_TEST_DIR "external_libs/googletest/googletest")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADERS)
set(SOURCE)
set(TEST_SOURCES)
set(SOURCE_DIR "source")

add_subdirectory(${SOURCE_DIR})

include_directories(${HEADERS})
include_directories(${G_TEST_SOURCE_DIR})
include_directories("external_libs/googletest/googletest")

message(STATUS ${SOURCE})
message(STATUS ${HEADERS})

set(PROGRAM_MAIN "source/src/main.cpp")
set(TEST_MAIN "external_libs/googletest/googletest/src/gtest_main.cc")

link_directories(${G_TEST_DIR}/my_build)

add_executable(compiler ${PROGRAM_MAIN} ${SOURCE})
target_link_libraries(compiler boost_iostreams)
#add_executable(gtest_exec ${TEST_MAIN} ${TEST_SOURCES}  ${SOURCE})
#target_link_libraries(gtest_exec gtest)
#target_link_libraries(gtest_exec pthread)
