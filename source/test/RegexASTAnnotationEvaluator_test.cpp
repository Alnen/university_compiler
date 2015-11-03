#ifndef REGEXASTANNOTATIONEVALUATOR_TEST_CPP
#define REGEXASTANNOTATIONEVALUATOR_TEST_CPP

#include "gtest/gtest.h"

#include <memory>

#include "RegexAST/RegexASTAnnotationEvaluator.h"
#include "RegexAST/RegexASTParser.h"
#include "RegexAST/RegexASTNode.h"
#include "InputChecker/InputHandler.h"
#include <Utility.h>

using namespace Lexer::RegexAST;
using namespace Lexer;

class RegexASTAnnotationEvaluatorTest : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        std::string regex("(a|b)*abb#");
        RegexToASTParser parser;
        RegexToASTParser::parse_asnswer_t answer = std::move(parser.parse(regex));
        ASSERT_TRUE(answer.m_success);
        ast = std::move(answer.m_ast);
    }
    virtual void TearDown() override
    {
        ast.reset(nullptr);
    }
    std::unique_ptr<BasicNode> ast;

};

TEST_F(RegexASTAnnotationEvaluatorTest, ifPassedCorrectASTSHouldCorrectlyEvaluateIt)
{
    RegexASTAnnotationEvaluator evaluator;
    RegexASTAnnotationEvaluator::answer_t followpos_table;
    ASSERT_NO_THROW( followpos_table = std::move(evaluator.evaluate(ast.get(), 1)); );
    ASSERT_EQ(6, followpos_table.second.size());
    std::vector<BasicLeaf::LeafId> firstpos = {1, 2, 3};
    std::vector<BasicLeaf::LeafId> lastpos = {6};
    ASSERT_EQ(firstpos, ast->m_firstpos);
    ASSERT_EQ(lastpos, ast->m_lastpos);
    //
    std::vector<std::vector<BasicLeaf::LeafId>> expected_table =
    {
        {1, 2, 3},
        {1, 2, 3},
        {4},
        {5},
        {6},
        {},
    };
    ASSERT_EQ(expected_table, followpos_table.second);
    //
    RegexASTAnnotationEvaluator::answer_t::first_type expected_node_to_id_mapping;
    expected_node_to_id_mapping[std::make_shared<CharChecker>('a')] = { 1, 3 };
    expected_node_to_id_mapping[std::make_shared<CharChecker>('b')] = { 2, 4, 5 };
    expected_node_to_id_mapping[std::make_shared<CharChecker>('#')] = { 6 };

    for(auto it1 = expected_node_to_id_mapping.begin(), it2 = followpos_table.first.begin(); it1 != expected_node_to_id_mapping.end() && it2 != followpos_table.first.end(); ++it1, ++it2)
    {
        ASSERT_EQ(*it1->first,  *it2->first);
        ASSERT_EQ(it1->second, it2->second);
    }
}

#endif // REGEXASTANNOTATIONEVALUATOR_TEST_CPP
