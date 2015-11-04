#ifndef REGEXASTTONFA_TEST_CPP
#define REGEXASTTONFA_TEST_CPP

#include "gtest/gtest.h"

#include <memory>

#include "RegexAST/RegexASTToNFA.h"
#include "RegexAST/RegexASTAnnotationEvaluator.h"
#include "RegexAST/RegexASTParser.h"
#include "RegexAST/RegexASTNode.h"
#include "FiniteAutomata/NFA.h"
#include "InputChecker/InputHandler.h"
#include "StdFix.h"

using namespace Lexer::RegexAST;
using namespace Lexer;

class RegexASTtoNFATest : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        std::string regex("(a|b)*abb#");
        RegexToASTParser parser;
        RegexToASTParser::parse_asnswer_t answer = std::move(parser.parse(regex));
        ASSERT_TRUE(answer.m_success);
        ast = std::move(answer.m_ast);

        RegexASTAnnotationEvaluator evaluator;

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
        expected_node_to_id_mapping[std::move(std::make_shared<CharChecker>('a'))] = { 1, 3 };
        expected_node_to_id_mapping[std::move(std::make_shared<CharChecker>('b'))] = { 2, 4, 5 };
        expected_node_to_id_mapping[std::move(std::make_shared<CharChecker>('#'))] = { 6 };

        for(auto it1 = expected_node_to_id_mapping.begin(), it2 = followpos_table.first.begin(); it1 != expected_node_to_id_mapping.end() && it2 != followpos_table.first.end(); ++it1, ++it2)
        {
            ASSERT_EQ(*it1->first,  *it2->first);
            ASSERT_EQ(it1->second, it2->second);
        }
    }
    virtual void TearDown() override
    {
        ast.reset(nullptr);
    }
    std::unique_ptr<BasicNode> ast;
    RegexASTAnnotationEvaluator::answer_t followpos_table;
};

TEST_F(RegexASTtoNFATest, ifPassedCorrectASTSHouldCorrectlyEvaluateIt)
{
    NFA<int, std::shared_ptr<IInputChecker>> nfa;
    nfa.addState(0);
    RegexASTtoNFA<int, std::shared_ptr<IInputChecker>> transformer(&nfa, 0);
    transformer.convertASTtoNFA(ast.get(), 1, followpos_table.first, followpos_table.second);
}

#endif // REGEXASTTONFA_TEST_CPP
