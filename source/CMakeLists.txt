set(SOURCE
    ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexASTNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexASTParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RegexASTToDFA.cpp
    PARENT_SCOPE
)

set(HEADERS
    ${HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PARENT_SCOPE
)

set(TEST_SOURCES
    ${TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/test/Map_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/DFA_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/RegexASTParser_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/RegexASTToDFA_test.cpp
    PARENT_SCOPE
)

message(STATUS "child :" ${SOURCE} ${HEADERS} ${TEST_SOURCES})
