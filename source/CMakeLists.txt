set(SOURCE
    ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexAST/RegexASTNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexAST/RegexASTParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexAST/RegexASTAnnotationEvaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/ArrayTypeInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/BasicTypeInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/CustomTypeInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/EnumerationTypeInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/LabelInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/Module.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/VarInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/IR/GlobalContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/Grammar/Terminals.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PascalCompiler/Grammar/Nonterminals.cpp
    PARENT_SCOPE
)

set(HEADERS
    ${HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PARENT_SCOPE
)

set(TEST_SOURCES
    ${TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/test/Utility_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/DFA_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/RegexASTParser_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/RegexASTToNFA_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/RegexASTAnnotationEvaluator_test.cpp
    PARENT_SCOPE
)

message(STATUS "child :" ${SOURCE} ${HEADERS} ${TEST_SOURCES})
