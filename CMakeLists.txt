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
set(CXX "/usr/local/Cellar/llvm/HEAD/bin/clan-3.8")
set(CUSTOM_LLVM_DIR "/usr/local/Cellar/llvm/HEAD/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CUSTOM_LLVM_DIR}  -fPIC -fvisibility-inlines-hidden -Wall -W -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wcovered-switch-default -Wnon-virtual-dtor -std=c++11 -fno-rtti -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/local/Cellar/llvm/HEAD/lib -Wl,-search_paths_first -Wl,-headerpad_max_install_names -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMInstrumentation -lLLVMProfileData -lLLVMTransformUtils -lLLVMBitWriter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMExecutionEngine -lLLVMTarget -lLLVMAnalysis -lLLVMRuntimeDyld -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMMC -lLLVMCore -lLLVMSupport -lcurses -lpthread -lz -lm")

add_subdirectory(${SOURCE_DIR})

include_directories(${CUSTOM_LLVM_DIR})
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
