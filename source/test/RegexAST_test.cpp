#ifndef REGEXAST_TEST_CPP
#define REGEXAST_TEST_CPP

#include "gtest/gtest.h"
#include "RegexAST.h"

#include <memory>

using namespace Lexer::RegexAST;

class RegexASTTest : public testing::Test
{

};

//parse_asnswer_t parse_char(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseChar_ifPassedSingleCharShouldSuccess)
{
    parse_asnswer_t result = parse_char("a", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(1, result.m_offset);
    CharCheckNode* ast = dynamic_cast<CharCheckNode*>(result.m_ast.get());
    EXPECT_NE(nullptr, ast);
    EXPECT_EQ('a', ast->m_elem);
}

TEST_F(RegexASTTest, parseChar_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_char("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}

//parse_asnswer_t parse_char_sequence_element(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedSingleCharShouldSuccess)
{
    parse_asnswer_t result = parse_char("a", 0);
    EXPECT_TRUE(result.m_success);
    CharCheckNode* ast = dynamic_cast<CharCheckNode*>(result.m_ast.get());
    EXPECT_NE(nullptr, ast);
    EXPECT_EQ('a', ast->m_elem);
    EXPECT_EQ(1, result.m_offset);
}

TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedCharRangeShouldSuccess)
{
    parse_asnswer_t result = parse_char_sequence_element("a-z", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(3, result.m_offset);
    CharRangeCheckNode* ast = dynamic_cast<CharRangeCheckNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ('a', ast->m_from);
    EXPECT_EQ('z', ast->m_to);

}

TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedEscapedCharShouldSuccess)
{
    parse_asnswer_t result = parse_char_sequence_element("\\]", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(2, result.m_offset);
    CharCheckNode* ast = dynamic_cast<CharCheckNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ(']', ast->m_elem);

}

TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_char_sequence_element("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}

//parse_asnswer_t parse_char_sequence(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseCharSequence_ifPassedSingleCharAndCharRangeShouldSuccess)
{
    parse_asnswer_t result = parse_char_sequence("ab-z", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(4, result.m_offset);
    CatNode* base_ast = dynamic_cast<CatNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    CharCheckNode* lhs_ast = dynamic_cast<CharCheckNode*>(base_ast->m_lhsElem.get());
    CharRangeCheckNode* rhs_ast = dynamic_cast<CharRangeCheckNode*>(base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, lhs_ast);
    ASSERT_NE(nullptr, rhs_ast);
    EXPECT_EQ('a', lhs_ast->m_elem);
    EXPECT_EQ('b', rhs_ast->m_from);
    EXPECT_EQ('z', rhs_ast->m_to);

}

TEST_F(RegexASTTest, parseCharSequence_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_char_sequence("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}

//parse_asnswer_t parse_char_class(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseCharClass_ifPassedSingleCharAndCharRangeShouldSuccess)
{
    parse_asnswer_t result = parse_char_class("[ab-z]", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(6, result.m_offset);
    CatNode* base_ast = dynamic_cast<CatNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    CharCheckNode* lhs_ast = dynamic_cast<CharCheckNode*>(base_ast->m_lhsElem.get());
    CharRangeCheckNode* rhs_ast = dynamic_cast<CharRangeCheckNode*>(base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, lhs_ast);
    ASSERT_NE(nullptr, rhs_ast);
    EXPECT_EQ('a', lhs_ast->m_elem);
    EXPECT_EQ('b', rhs_ast->m_from);
    EXPECT_EQ('z', rhs_ast->m_to);

}

TEST_F(RegexASTTest, parseCharClass_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_char_class("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}

//parse_asnswer_t parse_group(const std::string& regex, size_t offset);
// TODO

//parse_asnswer_t parse_single_element(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseSingleElement_ifPassedSingleCharShouldSuccess)
{
    parse_asnswer_t result = parse_single_element("a", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(1, result.m_offset);
    CharCheckNode* repeated_base_ast = dynamic_cast<CharCheckNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    EXPECT_EQ('a', repeated_base_ast->m_elem);
}

TEST_F(RegexASTTest, parseSingleElement_ifPassedCharClassShouldSuccess)
{
    parse_asnswer_t result = parse_single_element("[ab-z]", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(6, result.m_offset);
    CharClassCheckNode* class_base_ast = dynamic_cast<CharClassCheckNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, class_base_ast);
    CatNode* repeated_base_ast = dynamic_cast<CatNode*>(class_base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    CharCheckNode* lhs_ast = dynamic_cast<CharCheckNode*>(repeated_base_ast->m_lhsElem.get());
    CharRangeCheckNode* rhs_ast = dynamic_cast<CharRangeCheckNode*>(repeated_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, lhs_ast);
    ASSERT_NE(nullptr, rhs_ast);
    EXPECT_EQ('a', lhs_ast->m_elem);
    EXPECT_EQ('b', rhs_ast->m_from);
    EXPECT_EQ('z', rhs_ast->m_to);
}

TEST_F(RegexASTTest, parseSingleElement_ifPassedEscapedCharClassShouldSuccesss)
{
    parse_asnswer_t result = parse_single_element("\\n", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(2, result.m_offset);
    CharCheckNode* repeated_base_ast = dynamic_cast<CharCheckNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    EXPECT_EQ('n', repeated_base_ast->m_elem);
}

TEST_F(RegexASTTest, parseSingleElement_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_single_element("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}

//parse_asnswer_t parse_zero_or_more(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseZeroOrMore_ifPassedSingleCharShouldSuccess)
{
    parse_asnswer_t result = parse_zero_or_more("a*", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(2, result.m_offset);
    StarNode* base_ast = dynamic_cast<StarNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    CharCheckNode* repeated_base_ast = dynamic_cast<CharCheckNode*>(base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    EXPECT_EQ('a', repeated_base_ast->m_elem);
}

TEST_F(RegexASTTest, parseZeroOrMore_ifPassedCharClassShouldSuccess)
{
    parse_asnswer_t result = parse_zero_or_more("[ab-z]*", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(7, result.m_offset);
    StarNode* base_ast = dynamic_cast<StarNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    CharClassCheckNode* class_base_ast = dynamic_cast<CharClassCheckNode*>(base_ast->m_elem.get());
    ASSERT_NE(nullptr, class_base_ast);
    CatNode* repeated_base_ast = dynamic_cast<CatNode*>(class_base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    CharCheckNode* lhs_ast = dynamic_cast<CharCheckNode*>(repeated_base_ast->m_lhsElem.get());
    CharRangeCheckNode* rhs_ast = dynamic_cast<CharRangeCheckNode*>(repeated_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, lhs_ast);
    ASSERT_NE(nullptr, rhs_ast);
    EXPECT_EQ('a', lhs_ast->m_elem);
    EXPECT_EQ('b', rhs_ast->m_from);
    EXPECT_EQ('z', rhs_ast->m_to);

}

TEST_F(RegexASTTest, parseZeroOrMore_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_zero_or_more("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}

//parse_asnswer_t parse_sequence(const std::string& regex, size_t offset);

TEST_F(RegexASTTest, parseSequence_ifPassedCorrectSequenceShouldSuccess)
{
    parse_asnswer_t result = parse_sequence("a[ab-z]*d", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(9, result.m_offset);
    CatNode* top_base_ast = dynamic_cast<CatNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, top_base_ast);
    CatNode* bottom_base_ast = dynamic_cast<CatNode*>(top_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, bottom_base_ast);
    //first
    CharCheckNode* first_check = dynamic_cast<CharCheckNode*>(bottom_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, first_check);
    EXPECT_EQ('a', first_check->m_elem);
    //2nd
    StarNode* second_check = dynamic_cast<StarNode*>(bottom_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, second_check);
    CharClassCheckNode* class_base_ast = dynamic_cast<CharClassCheckNode*>(second_check->m_elem.get());
    ASSERT_NE(nullptr, class_base_ast);
    CatNode* repeated_base_ast = dynamic_cast<CatNode*>(class_base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    CharCheckNode* lhs_ast = dynamic_cast<CharCheckNode*>(repeated_base_ast->m_lhsElem.get());
    CharRangeCheckNode* rhs_ast = dynamic_cast<CharRangeCheckNode*>(repeated_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, lhs_ast);
    ASSERT_NE(nullptr, rhs_ast);
    EXPECT_EQ('a', lhs_ast->m_elem);
    EXPECT_EQ('b', rhs_ast->m_from);
    EXPECT_EQ('z', rhs_ast->m_to);
    //
    CharCheckNode* third_check = dynamic_cast<CharCheckNode*>(top_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, third_check);
    EXPECT_EQ('d', third_check->m_elem);
}

TEST_F(RegexASTTest, parseSequence_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_sequence("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}
#include <iostream>
//parse_asnswer_t parse_or(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseOr_ifPassedCharClassShouldSuccess)
{
    std::string regex("(a[ab-z]*d|d)");
    parse_asnswer_t result = parse_or(regex, 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(regex.size(), result.m_offset);
    OrNode* topmost_base_ast = dynamic_cast<OrNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, topmost_base_ast);

    CatNode* top_base_ast = dynamic_cast<CatNode*>(topmost_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, top_base_ast);
    CatNode* bottom_base_ast = dynamic_cast<CatNode*>(top_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, bottom_base_ast);
    //first
    CharCheckNode* first_check = dynamic_cast<CharCheckNode*>(bottom_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, first_check);
    EXPECT_EQ('a', first_check->m_elem);
    //second
    StarNode* second_check = dynamic_cast<StarNode*>(bottom_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, second_check);
    CharClassCheckNode* class_base_ast = dynamic_cast<CharClassCheckNode*>(second_check->m_elem.get());
    ASSERT_NE(nullptr, class_base_ast);
    CatNode* repeated_base_ast = dynamic_cast<CatNode*>(class_base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    CharCheckNode* lhs_ast = dynamic_cast<CharCheckNode*>(repeated_base_ast->m_lhsElem.get());
    CharRangeCheckNode* rhs_ast = dynamic_cast<CharRangeCheckNode*>(repeated_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, lhs_ast);
    ASSERT_NE(nullptr, rhs_ast);
    EXPECT_EQ('a', lhs_ast->m_elem);
    EXPECT_EQ('b', rhs_ast->m_from);
    EXPECT_EQ('z', rhs_ast->m_to);
    //third
    CharCheckNode* third_check = dynamic_cast<CharCheckNode*>(top_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, third_check);
    EXPECT_EQ('d', third_check->m_elem);
}

TEST_F(RegexASTTest, parseOr_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_or("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}

//parse_asnswer_t parse_regex(const std::string& regex);
TEST_F(RegexASTTest, parseRegex_ifPassedCharClassShouldSuccess)
{
    std::string regex("[a-zA-Z][a-zA-Z0-9]*");
    parse_asnswer_t result = parse_or(regex, 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(regex.size(), result.m_offset);
}

TEST_F(RegexASTTest, parseRegex_ifPassedCharClassShouldSuccess1)
{
    std::string regex("[a-zA-Z][a-zA-Z0-9]*");
    parse_asnswer_t result = parse_or(regex, 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(regex.size(), result.m_offset);
}


TEST_F(RegexASTTest, parseRegex_ifPassedEmptyStringShouldFail)
{
    parse_asnswer_t result = parse_or("", 0);
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, result.m_offset);
}

#endif // REGEXAST_TEST_CPP
