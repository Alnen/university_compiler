#ifndef REGEXASTPARSER_TEST_CPP
#define REGEXASTPARSER_TEST_CPP

#include "gtest/gtest.h"
#include "RegexAST/RegexASTParser.h"
#include "StdFix.h"

#include <memory>

using namespace Lexer::RegexAST;

class RegexToASTParserTest : public RegexToASTParser
{
public:
    void resetIdGenerator() { m_idGenerator = 0; }
    void resetOffset() { m_offset = 0; }
    size_t offset() { return m_offset; }

    bool parse_char_sequence_element(const std::string& regex, CharClassCheckLeaf& node) { return RegexToASTParser::parse_char_sequence_element(regex, node); }
    bool parse_char_sequence(const std::string& regex, CharClassCheckLeaf& node) { return RegexToASTParser::parse_char_sequence(regex, node); }
    parse_asnswer_t parse_char_class(const std::string& regex) { return RegexToASTParser::parse_char_class(regex); }
    parse_asnswer_t parse_char(const std::string& regex) { return RegexToASTParser::parse_char(regex); }
    parse_asnswer_t parse_group(const std::string& regex) { return RegexToASTParser::parse_group(regex); }
    parse_asnswer_t parse_single_element(const std::string& regex) { return RegexToASTParser::parse_single_element(regex); }
    parse_asnswer_t parse_zero_or_more(const std::string& regex) { return RegexToASTParser::parse_zero_or_more(regex); }
    parse_asnswer_t parse_sequence(const std::string& regex) { return RegexToASTParser::parse_sequence(regex); }
    parse_asnswer_t parse_or(const std::string& regex) { return RegexToASTParser::parse_or(regex); }
};

class RegexASTTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        parser.resetIdGenerator();
        parser.resetOffset();
    }

    RegexToASTParserTest parser;
};

//RegexToASTParser::parse_asnswer_t RegexToASTParser::parse_char(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseChar_ifPassedSingleCharShouldSuccess)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_char("a");
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(1, parser.offset());
    CharCheckLeaf* ast = dynamic_cast<CharCheckLeaf*>(result.m_ast.get());
    EXPECT_NE(nullptr, ast);
    EXPECT_EQ('a', ast->m_elem);
}

TEST_F(RegexASTTest, parseChar_ifPassedEmptyStringShouldFail)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_char("");
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, parser.offset());
}

//RegexToASTParser::parse_asnswer_t parse_char_sequence_element(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedSingleCharShouldSuccess)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_char("a");
    EXPECT_TRUE(result.m_success);
    CharCheckLeaf* ast = dynamic_cast<CharCheckLeaf*>(result.m_ast.get());
    EXPECT_NE(nullptr, ast);
    EXPECT_EQ('a', ast->m_elem);
    EXPECT_EQ(1, parser.offset());
}

TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedCharRangeShouldSuccess)
{
    std::string regex("a-z");
    std::unique_ptr<CharClassCheckLeaf> char_class = std::make_unique<CharClassCheckLeaf>( 0 );
    EXPECT_TRUE(parser.parse_char_sequence_element("a-z", *char_class.get()));
    EXPECT_EQ(regex.size(), parser.offset());
    EXPECT_EQ(0, char_class->m_charChecks.size());
    ASSERT_EQ(1, char_class->m_charRangeChecks.size());
    EXPECT_EQ('a', char_class->m_charRangeChecks[0].first);
    EXPECT_EQ('z', char_class->m_charRangeChecks[0].second);

}

TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedEscapedCharShouldSuccess)
{
    std::string regex("\\]");
    std::unique_ptr<CharClassCheckLeaf> char_class = std::make_unique<CharClassCheckLeaf>( 0 );
    EXPECT_TRUE(parser.parse_char_sequence_element(regex, *char_class.get()));
    EXPECT_EQ(regex.size(), parser.offset());
    EXPECT_EQ(1, char_class->m_charChecks.size());
    ASSERT_EQ(0, char_class->m_charRangeChecks.size());
    EXPECT_EQ(']', char_class->m_charChecks[0]);
}

TEST_F(RegexASTTest, parseCharSequenceElement_ifPassedEmptyStringShouldFail)
{
    std::string regex("");
    std::unique_ptr<CharClassCheckLeaf> char_class = std::make_unique<CharClassCheckLeaf>( 0 );
    EXPECT_FALSE(parser.parse_char_sequence_element(regex, *char_class.get()));
    EXPECT_EQ(0, parser.offset());
    EXPECT_EQ(0, char_class->m_charChecks.size());
    ASSERT_EQ(0, char_class->m_charRangeChecks.size());
}

//RegexToASTParser::parse_asnswer_t parse_char_sequence(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseCharSequence_ifPassedSingleCharAndCharRangeShouldSuccess)
{
    std::string regex("ab-z");
    std::unique_ptr<CharClassCheckLeaf> char_class = std::make_unique<CharClassCheckLeaf>( 0 );
    EXPECT_TRUE(parser.parse_char_sequence("ab-z", *char_class.get()));
    EXPECT_EQ(regex.size(), parser.offset());
    EXPECT_EQ(1, char_class->m_charChecks.size());
    ASSERT_EQ(1, char_class->m_charRangeChecks.size());
    EXPECT_EQ('a', char_class->m_charChecks[0]);
    EXPECT_EQ('b', char_class->m_charRangeChecks[0].first);
    EXPECT_EQ('z', char_class->m_charRangeChecks[0].second);

}

TEST_F(RegexASTTest, parseCharSequence_ifPassedEmptyStringShouldFail)
{
    std::unique_ptr<CharClassCheckLeaf> char_class = std::make_unique<CharClassCheckLeaf>( 0 );
    EXPECT_FALSE(parser.parse_char_sequence("", *char_class.get()));
    EXPECT_EQ(0, parser.offset());
}

//RegexToASTParser::parse_asnswer_t parse_char_class(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseCharClass_ifPassedSingleCharAndCharRangeShouldSuccess)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_char_class("[ab-z]");
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(6, parser.offset());
    CharClassCheckLeaf* char_class = dynamic_cast<CharClassCheckLeaf*>(result.m_ast.get());
    ASSERT_NE(nullptr, char_class);
    EXPECT_EQ(1, char_class->m_charChecks.size());
    ASSERT_EQ(1, char_class->m_charRangeChecks.size());
    EXPECT_EQ('a', char_class->m_charChecks[0]);
    EXPECT_EQ('b', char_class->m_charRangeChecks[0].first);
    EXPECT_EQ('z', char_class->m_charRangeChecks[0].second);

}

TEST_F(RegexASTTest, parseCharClass_ifPassedEmptyStringShouldFail)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_char_class("");
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, parser.offset());
}

//RegexToASTParser::parse_asnswer_t parse_group(const std::string& regex, size_t offset);
// TODO

//RegexToASTParser::parse_asnswer_t parse_single_element(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseSingleElement_ifPassedSingleCharShouldSuccess)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_single_element("a");
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(1, parser.offset());
    CharCheckLeaf* repeated_base_ast = dynamic_cast<CharCheckLeaf*>(result.m_ast.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    EXPECT_EQ('a', repeated_base_ast->m_elem);
}

TEST_F(RegexASTTest, parseSingleElement_ifPassedCharClassShouldSuccess)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_single_element("[ab-z]");
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(6, parser.offset());
    CharClassCheckLeaf* char_class = dynamic_cast<CharClassCheckLeaf*>(result.m_ast.get());
    ASSERT_NE(nullptr, char_class);
    EXPECT_EQ(1, char_class->m_charChecks.size());
    ASSERT_EQ(1, char_class->m_charRangeChecks.size());
    EXPECT_EQ('a', char_class->m_charChecks[0]);
    EXPECT_EQ('b', char_class->m_charRangeChecks[0].first);
    EXPECT_EQ('z', char_class->m_charRangeChecks[0].second);
}

TEST_F(RegexASTTest, parseSingleElement_ifPassedEscapedCharClassShouldSuccesss)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_single_element("\\n");
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(2, parser.offset());
    CharCheckLeaf* repeated_base_ast = dynamic_cast<CharCheckLeaf*>(result.m_ast.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    EXPECT_EQ('n', repeated_base_ast->m_elem);
}

TEST_F(RegexASTTest, parseSingleElement_ifPassedEmptyStringShouldFail)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_single_element("");
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, parser.offset());
}

//RegexToASTParser::parse_asnswer_t parse_zero_or_more(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseZeroOrMore_ifPassedSingleCharShouldSuccess)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_zero_or_more("a*");
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(2, parser.offset());
    StarNode* base_ast = dynamic_cast<StarNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    CharCheckLeaf* repeated_base_ast = dynamic_cast<CharCheckLeaf*>(base_ast->m_elem.get());
    ASSERT_NE(nullptr, repeated_base_ast);
    EXPECT_EQ('a', repeated_base_ast->m_elem);
}

TEST_F(RegexASTTest, parseZeroOrMore_ifPassedCharClassShouldSuccess)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_zero_or_more("[ab-z]*");
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(7, parser.offset());
    StarNode* base_ast = dynamic_cast<StarNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, base_ast);
    CharClassCheckLeaf* char_class = dynamic_cast<CharClassCheckLeaf*>(base_ast->m_elem.get());
    ASSERT_NE(nullptr, char_class);
    EXPECT_EQ(1, char_class->m_charChecks.size());
    ASSERT_EQ(1, char_class->m_charRangeChecks.size());
    EXPECT_EQ('a', char_class->m_charChecks[0]);
    EXPECT_EQ('b', char_class->m_charRangeChecks[0].first);
    EXPECT_EQ('z', char_class->m_charRangeChecks[0].second);

}

TEST_F(RegexASTTest, parseZeroOrMore_ifPassedEmptyStringShouldFail)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_zero_or_more("");
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, parser.offset());
}

//RegexToASTParser::parse_asnswer_t parse_sequence(const std::string& regex, size_t offset);

TEST_F(RegexASTTest, parseSequence_ifPassedCorrectSequenceShouldSuccess)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_sequence("a[ab-z]*d");
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(9, parser.offset());
    CatNode* top_base_ast = dynamic_cast<CatNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, top_base_ast);
    CatNode* bottom_base_ast = dynamic_cast<CatNode*>(top_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, bottom_base_ast);
    //first
    CharCheckLeaf* first_check = dynamic_cast<CharCheckLeaf*>(bottom_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, first_check);
    EXPECT_EQ('a', first_check->m_elem);
    //2nd
    StarNode* second_check = dynamic_cast<StarNode*>(bottom_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, second_check);
    CharClassCheckLeaf* char_class = dynamic_cast<CharClassCheckLeaf*>(second_check->m_elem.get());
    ASSERT_NE(nullptr, char_class);
    EXPECT_EQ(1, char_class->m_charChecks.size());
    ASSERT_EQ(1, char_class->m_charRangeChecks.size());
    EXPECT_EQ('a', char_class->m_charChecks[0]);
    EXPECT_EQ('b', char_class->m_charRangeChecks[0].first);
    EXPECT_EQ('z', char_class->m_charRangeChecks[0].second);
    //
    CharCheckLeaf* third_check = dynamic_cast<CharCheckLeaf*>(top_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, third_check);
    EXPECT_EQ('d', third_check->m_elem);
}

TEST_F(RegexASTTest, parseSequence_ifPassedEmptyStringShouldFail)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_sequence("");
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, parser.offset());
}
#include <iostream>
//RegexToASTParser::parse_asnswer_t parse_or(const std::string& regex, size_t offset);
TEST_F(RegexASTTest, parseOr_ifPassedCharClassShouldSuccess)
{
    std::string regex("(a[ab-z]*d|d)");
    RegexToASTParser::parse_asnswer_t result = parser.parse_or(regex);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(regex.size(), parser.offset());
    OrNode* topmost_base_ast = dynamic_cast<OrNode*>(result.m_ast.get());
    ASSERT_NE(nullptr, topmost_base_ast);

    CatNode* top_base_ast = dynamic_cast<CatNode*>(topmost_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, top_base_ast);
    CatNode* bottom_base_ast = dynamic_cast<CatNode*>(top_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, bottom_base_ast);
    //first
    CharCheckLeaf* first_check = dynamic_cast<CharCheckLeaf*>(bottom_base_ast->m_lhsElem.get());
    ASSERT_NE(nullptr, first_check);
    EXPECT_EQ('a', first_check->m_elem);
    //second
    StarNode* second_check = dynamic_cast<StarNode*>(bottom_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, second_check);
    CharClassCheckLeaf* char_class = dynamic_cast<CharClassCheckLeaf*>(second_check->m_elem.get());
    ASSERT_NE(nullptr, char_class);
    EXPECT_EQ(1, char_class->m_charChecks.size());
    ASSERT_EQ(1, char_class->m_charRangeChecks.size());
    EXPECT_EQ('a', char_class->m_charChecks[0]);
    EXPECT_EQ('b', char_class->m_charRangeChecks[0].first);
    EXPECT_EQ('z', char_class->m_charRangeChecks[0].second);
    //third
    CharCheckLeaf* third_check = dynamic_cast<CharCheckLeaf*>(top_base_ast->m_rhsElem.get());
    ASSERT_NE(nullptr, third_check);
    EXPECT_EQ('d', third_check->m_elem);
}

TEST_F(RegexASTTest, parseOr_ifPassedEmptyStringShouldFail)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_or("");
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, parser.offset());
}

//RegexToASTParser::parse_asnswer_t parse_regex(const std::string& regex);
TEST_F(RegexASTTest, parseRegex_ifPassedCharClassShouldSuccess)
{
    std::string regex("[a-zA-Z][a-zA-Z0-9]*");
    RegexToASTParser::parse_asnswer_t result = parser.parse_or(regex);
    EXPECT_TRUE(result.m_success);
    EXPECT_EQ(regex.size(), parser.offset());
}

TEST_F(RegexASTTest, parseRegex_ifPassedEmptyStringShouldFail)
{
    RegexToASTParser::parse_asnswer_t result = parser.parse_or("");
    EXPECT_FALSE(result.m_success);
    EXPECT_EQ(0, parser.offset());
}

#endif // REGEXASTPARSER_TEST_CPP
