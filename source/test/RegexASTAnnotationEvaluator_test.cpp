#ifndef REGEXASTANNOTATIONEVALUATOR_TEST_CPP
#define REGEXASTANNOTATIONEVALUATOR_TEST_CPP

#include "gtest/gtest.h"
#include "RegexASTAnnotationEvaluator.h"
#include "RegexASTParser.h"
#include "RegexASTNode.h"
#include <memory>

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
    expected_node_to_id_mapping[std::move(std::make_unique<CharChecker>('a'))] = { 1, 3 };
    expected_node_to_id_mapping[std::move(std::make_unique<CharChecker>('b'))] = { 2, 4, 5 };
    expected_node_to_id_mapping[std::move(std::make_unique<CharChecker>('#'))] = { 6 };

    for(auto it1 = expected_node_to_id_mapping.begin(), it2 = followpos_table.first.begin(); it1 != expected_node_to_id_mapping.end() && it2 != followpos_table.first.end(); ++it1, ++it2)
    {
        ASSERT_EQ(*it1->get<0>(), *it2->get<0>());
        ASSERT_EQ(it1->get<1>(), it2->get<1>());
    }
}

#endif // REGEXASTANNOTATIONEVALUATOR_TEST_CPP