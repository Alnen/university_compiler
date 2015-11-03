set(SOURCE
    ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexAST/RegexASTNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexAST/RegexASTParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexAST/RegexASTAnnotationEvaluator.cpp
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
