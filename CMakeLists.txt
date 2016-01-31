cmake_minimum_required (VERSION 2.6)
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
message(STATUS "HANA_INCLUDE_DIR: \"$ENV{HANA_INCLUDE_DIR}\"")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${LLVM_INCLUDE_DIR} -I${HANA_INCLUDE_DIR} -I${LLVM_C_INCLUDE_DIR} -fPIC -fvisibility-inlines-hidden -Wall -W -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wcovered-switch-default -Wnon-virtual-dtor -fno-rtti -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/local/Cellar/llvm/HEAD/lib -Wl,-search_paths_first -Wl,-headerpad_max_install_names -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMInstrumentation -lLLVMProfileData -lLLVMTransformUtils -lLLVMBitWriter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMExecutionEngine -lLLVMTarget -lLLVMAnalysis -lLLVMRuntimeDyld -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMMC -lLLVMCore -lLLVMSupport -lcurses -lpthread -lz -lm")

set(CMAKE_CXX_FLAGS "${LLVM_CXX_CONFIG} -I$ENV{HANA_INCLUDE_DIR} ${CMAKE_CXX_FLAGS} ${LLVM_CXX_LINK_CONFIG} -fexceptions")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKGER_FLAGS} ${LLVM_CXX_LINK_DIR}")
message(STATUS "CMAKE_CXX_FLAGS= " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_EXE_LINKER_FLAGS= " ${CMAKE_EXE_LINKER_FLAGS})
add_subdirectory(${SOURCE_DIR})

include_directories(${CUSTOM_LLVM_DIR})
include_directories(${HEADERS})
include_directories(${G_TEST_SOURCE_DIR})
include_directories("external_libs/googletest/googletest")
include_directories("/Users/alnen/Desktop/Coding/C++ projects/hana/include")

message(STATUS ${SOURCE})
message(STATUS ${HEADERS})

set(PROGRAM_MAIN "source/src/main.cpp")
set(TEST_MAIN "external_libs/googletest/googletest/src/gtest_main.cc")

link_directories(${G_TEST_DIR}/my_build)

add_executable(compiler ${PROGRAM_MAIN} ${SOURCE})

IF(APPLE)
  target_link_libraries(compiler boost_iostreams)
endif()
foreach(LIB ${LLVM_CXX_LINK_LIBS})
  target_link_libraries(compiler "${LIB}")
endforeach(LIB)
#target_link_libraries(compiler LLVMCore)
#target_link_libraries(compiler LLVMSupport)
#target_link_libraries(compiler z)
#target_link_libraries(compiler pthread)
#target_link_libraries(compiler ffi)
#target_link_libraries(compiler edit)
#target_link_libraries(compiler tinfo)
#target_link_libraries(compiler dl)
#target_link_libraries(compiler m)
#add_executable(gtest_exec ${TEST_MAIN} ${TEST_SOURCES}  ${SOURCE})

#target_link_libraries(gtest_exec gtest)
#target_link_libraries(gtest_exec pthread)
