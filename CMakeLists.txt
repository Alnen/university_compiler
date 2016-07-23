cmake_minimum_required (VERSION 3.3)
project (Compiler)

set(G_TEST_SOURCE_DIR "external_libs/googletest/googletest/include")
set(G_TEST_DIR "external_libs/googletest/googletest")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(HEADERS)
set(SOURCE)
set(TEST_SOURCES)
set(SOURCE_DIR "source")
#set(CXX "/usr/local/Cellar/llvm/HEAD/bin/clan-3.8")
#set(CUSTOM_LLVM_DIR "/usr/local/Cellar/llvm/HEAD/include")


exec_program(llvm-config ARGS --cxxflags OUTPUT_VARIABLE LLVM_CXX_CONFIG)
exec_program(llvm-config ARGS --ldflags OUTPUT_VARIABLE LLVM_CXX_LINK_DIR)
exec_program(llvm-config ARGS --system-libs --libs core OUTPUT_VARIABLE LLVM_CXX_LINK_LIBS)

STRING(REGEX REPLACE "\n" " " LLVM_CXX_CONFIG ${LLVM_CXX_CONFIG})
STRING(REGEX REPLACE "\n" " " LLVM_CXX_LINK_DIR ${LLVM_CXX_LINK_DIR})
STRING(REGEX REPLACE "\n" " " LLVM_CXX_LINK_LIBS ${LLVM_CXX_LINK_LIBS})
STRING(REPLACE "-l" "" LLVM_CXX_LINK_LIBS ${LLVM_CXX_LINK_LIBS})
STRING(REPLACE " " ";" LLVM_CXX_LINK_LIBS ${LLVM_CXX_LINK_LIBS})

message(STATUS "LLVM_CXX_CONFIG: \"${LLVM_CXX_CONFIG}\"")
message(STATUS "LLVM_CXX_LINK_DIR: \"${LLVM_CXX_LINK_DIR}\"")
message(STATUS "LLVM_CXX_LINK_LIBS: \"${LLVM_CXX_LINK_LIBS}\"")

set(CMAKE_CXX_FLAGS "${LLVM_CXX_CONFIG} ${CMAKE_CXX_FLAGS} ${LLVM_CXX_LINK_CONFIG} -fexceptions")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKGER_FLAGS} ${LLVM_CXX_LINK_DIR}")
message(STATUS "CMAKE_CXX_FLAGS= " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_EXE_LINKER_FLAGS= " ${CMAKE_EXE_LINKER_FLAGS})
add_subdirectory(${SOURCE_DIR})

include_directories(${CUSTOM_LLVM_DIR})
include_directories(${HEADERS})
include_directories(${G_TEST_SOURCE_DIR})
add_subdirectory("external_libs/googletest")

message(WARNING ${SOURCE})
message(WARNING ${HEADERS})

set(PROGRAM_MAIN "source/src/main.cpp")

link_directories(${G_TEST_DIR}/my_build)

add_executable(compiler ${PROGRAM_MAIN} ${SOURCE})

target_link_libraries(compiler boost_iostreams)

add_executable(gtest_exec ${TEST_SOURCES}  ${SOURCE})
target_link_libraries(gtest_exec gtest gtest_main)
foreach(LIB ${LLVM_CXX_LINK_LIBS})
  target_link_libraries(compiler   "${LIB}")
  target_link_libraries(gtest_exec "${LIB}")
endforeach(LIB)

target_link_libraries(gtest_exec pthread)
