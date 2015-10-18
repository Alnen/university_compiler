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
    char_check* ast = dynamic_cast<char_check*>(result.m_ast.get());
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
    char_check* ast = dynamic_cast<char_check*>(result.m_ast.get());
    EXPECT_NE(nullptr, ast);
    EXPECT_EQ('a', ast->m_elem);
    EXPECT_EQ(1, result.m_offset);
}

TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedCharRangeShouldSuccess)
{
    parse_asnswer_t result = parse_char_sequence_element("a-z", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(3, result.m_offset);
    char_range_check* ast = dynamic_cast<char_range_check*>(result.m_ast.get());
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ('a', ast->m_from);
    EXPECT_EQ('z', ast->m_to);

}

TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedEscapedCharShouldSuccess)
{
    parse_asnswer_t result = parse_char_sequence_element("\\]", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(2, result.m_offset);
    char_check* ast = dynamic_cast<char_check*>(result.m_ast.get());
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
    cat_op* base_ast = dynamic_cast<cat_op*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    char_check* lhs_ast = dynamic_cast<char_check*>(base_ast->m_lhsElem.get());
    char_range_check* rhs_ast = dynamic_cast<char_range_check*>(base_ast->m_rhsElem.get());
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
    cat_op* base_ast = dynamic_cast<cat_op*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    char_check* lhs_ast = dynamic_cast<char_check*>(base_ast->m_lhsElem.get());
    char_range_check* rhs_ast = dynamic_cast<char_range_check*>(base_ast->m_rhsElem.get());
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
    char_check* repeated_base_ast = dynamic_cast<char_check*>(result.m_ast.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    EXPECT_EQ('a', repeated_base_ast->m_elem);
}

TEST_F(RegexASTTest, parseSingleElement_ifPassedCharClassShouldSuccess)
{
    parse_asnswer_t result = parse_single_element("[ab-z]", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(6, result.m_offset);
    char_class_check* class_base_ast = dynamic_cast<char_class_check*>(result.m_ast.get());
    ASSERT_NE(nullptr, class_base_ast);
    cat_op* repeated_base_ast = dynamic_cast<cat_op*>(class_base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    char_check* lhs_ast = dynamic_cast<char_check*>(repeated_base_ast->m_lhsElem.get());
    char_range_check* rhs_ast = dynamic_cast<char_range_check*>(repeated_base_ast->m_rhsElem.get());
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
    char_check* repeated_base_ast = dynamic_cast<char_check*>(result.m_ast.get());
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
    star_op* base_ast = dynamic_cast<star_op*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    char_check* repeated_base_ast = dynamic_cast<char_check*>(base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    EXPECT_EQ('a', repeated_base_ast->m_elem);
}

TEST_F(RegexASTTest, parseZeroOrMore_ifPassedCharClassShouldSuccess)
{
    parse_asnswer_t result = parse_zero_or_more("[ab-z]*", 0);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(7, result.m_offset);
    star_op* base_ast = dynamic_cast<star_op*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    char_class_check* class_base_ast = dynamic_cast<char_class_check*>(base_ast->m_elem.get());
    ASSERT_NE(nullptr, class_base_ast);
    cat_op* repeated_base_ast = dynamic_cast<cat_op*>(class_base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    char_check* lhs_ast = dynamic_cast<char_check*>(repeated_base_ast->m_lhsElem.get());
    char_range_check* rhs_ast = dynamic_cast<char_range_check*>(repeated_base_ast->m_rhsElem.get());
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
    cat_op* top_base_ast = dynamic_cast<cat_op*>(result.m_ast.get());
    ASSERT_NE(nullptr, top_base_ast);
    cat_op* bottom_base_ast = dynamic_cast<cat_op*>(top_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, bottom_base_ast);
    //first
    char_check* first_check = dynamic_cast<char_check*>(bottom_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, first_check);
    EXPECT_EQ('a', first_check->m_elem);
    //2nd
    star_op* second_check = dynamic_cast<star_op*>(bottom_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, second_check);
    char_class_check* class_base_ast = dynamic_cast<char_class_check*>(second_check->m_elem.get());
    ASSERT_NE(nullptr, class_base_ast);
    cat_op* repeated_base_ast = dynamic_cast<cat_op*>(class_base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    char_check* lhs_ast = dynamic_cast<char_check*>(repeated_base_ast->m_lhsElem.get());
    char_range_check* rhs_ast = dynamic_cast<char_range_check*>(repeated_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, lhs_ast);
    ASSERT_NE(nullptr, rhs_ast);
    EXPECT_EQ('a', lhs_ast->m_elem);
    EXPECT_EQ('b', rhs_ast->m_from);
    EXPECT_EQ('z', rhs_ast->m_to);
    //
    char_check* third_check = dynamic_cast<char_check*>(top_base_ast->m_rhsElem.get());
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
    or_op* topmost_base_ast = dynamic_cast<or_op*>(result.m_ast.get());
    ASSERT_NE(nullptr, topmost_base_ast);

    cat_op* top_base_ast = dynamic_cast<cat_op*>(topmost_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, top_base_ast);
    cat_op* bottom_base_ast = dynamic_cast<cat_op*>(top_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, bottom_base_ast);
    //first
    char_check* first_check = dynamic_cast<char_check*>(bottom_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, first_check);
    EXPECT_EQ('a', first_check->m_elem);
    //second
    star_op* second_check = dynamic_cast<star_op*>(bottom_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, second_check);
    char_class_check* class_base_ast = dynamic_cast<char_class_check*>(second_check->m_elem.get());
    ASSERT_NE(nullptr, class_base_ast);
    cat_op* repeated_base_ast = dynamic_cast<cat_op*>(class_base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    char_check* lhs_ast = dynamic_cast<char_check*>(repeated_base_ast->m_lhsElem.get());
    char_range_check* rhs_ast = dynamic_cast<char_range_check*>(repeated_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, lhs_ast);
    ASSERT_NE(nullptr, rhs_ast);
    EXPECT_EQ('a', lhs_ast->m_elem);
    EXPECT_EQ('b', rhs_ast->m_from);
    EXPECT_EQ('z', rhs_ast->m_to);
    //third
    char_check* third_check = dynamic_cast<char_check*>(top_base_ast->m_rhsElem.get());
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
