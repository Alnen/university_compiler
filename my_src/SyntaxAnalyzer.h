#pragma once

#include "Grammar.h"
#include "ControlTable.h"
#include <vector>

namespace Parser
{

template<class _TerminalType, class _NonterminalType>
class SyntaxAnalyzer
{
public:
    using TeminalType    = typename Grammar<_TerminalType, _NonterminalType>::TerminalType;
    using NonteminalType = typename Grammar<_TerminalType, _NonterminalType>::NonterminalType;
    using TokenType      = typename Grammar<_TerminalType, _NonterminalType>::TokenType;
    using TokenList      = typename Grammar<_TerminalType, _NonterminalType>::TokenList;

    SyntaxAnalyzer(const Grammar<_TerminalType, _NonterminalType>& grammar);
	int readNextToken(TokenType new_token);
    int readTokens(TokenList tokens);

private:
    int getRuleId(const Rule<TokenType>& rule);
	
private:
    ControlTable<_TerminalType, _NonterminalType> m_control_table;
    Grammar<_TerminalType, _NonterminalType> m_grammar;
    TokenList m_stack;
    //
    std::vector<int> output_tree;
};

template<class TerminalType, class NonterminalType>
SyntaxAnalyzer<TerminalType, NonterminalType>::SyntaxAnalyzer(const Grammar<TerminalType, NonterminalType>& grammar) : m_grammar(grammar), m_control_table(grammar)
{
    m_stack.push_back(TerminalType::ENDOFFILE);
    m_stack.push_back(grammar.start_symbol);
}

template<class TerminalType, class NonterminalType>
int SyntaxAnalyzer<TerminalType, NonterminalType>::readNextToken(TokenType new_token)
{
    if (m_stack[m_stack.size() - 1] == TerminalType::ENDOFFILE) {
        return 0;
    }
    if (m_stack[m_stack.size() - 1] == new_token) {
        m_stack.pop_back();
        if (m_stack[m_stack.size() - 1] == TerminalType::ENDOFFILE) {
            return 0;
        }
        return 1;
    }
    else if (m_grammar.isTerminal(m_stack[m_stack.size() - 1])) {
        return -1;
    }
    Rule<TokenType> ct_rule = m_control_table.getRule(m_stack[m_stack.size() - 1], new_token);
    if (ct_rule.left < 0) {
        return -1;
    }
    else {
        output_tree.push_back(getRuleId(ct_rule));
        m_stack.pop_back();
        for (int j = ct_rule.right.size() - 1; j >= 0; --j) {
            m_stack.push_back(ct_rule.right[j]);
        }
        if (m_stack[m_stack.size() - 1] == TerminalType::ENDOFFILE) {
            return 0;
        }
    }
    return 1;
}

template<class TerminalType, class NonterminalType>
int SyntaxAnalyzer<TerminalType, NonterminalType>::readTokens(TokenList tokens)
{
    size_t i = 0;
    Rule<TokenType> ct_rule;
    while (i < tokens.size()) {
        if (m_stack[m_stack.size() - 1] == TerminalType::ENDOFFILE) {
            return 0;
        }
        if (m_stack[m_stack.size() - 1] == tokens[i]) {
            m_stack.pop_back();
            ++i;
            continue;
        }
        else if (m_grammar.isTerminal(m_stack[m_stack.size() - 1])) {
            return -1;
        }
        ct_rule = m_control_table.getRule(m_stack[m_stack.size() - 1], tokens[i]);
        if (ct_rule.left < 0) {
            return -1;
        }
        else {
            output_tree.push_back(getRuleId(ct_rule));
            m_stack.pop_back();
            for (int j = ct_rule.right.size() - 1; j >= 0; --j) {
                m_stack.push_back(ct_rule.right[j]);
            }
        }
    }
    if (m_stack[m_stack.size() - 1] == TerminalType::ENDOFFILE) {
        return 0;
    }
    return 1;
}

template<class TerminalType, class NonterminalType>
int SyntaxAnalyzer<TerminalType, NonterminalType>::getRuleId(const Rule<TokenType>& rule)
{
    for (int i = 0; i < m_grammar.rules.size(); ++i) {
        if (m_grammar.rules[i] == rule) return i;
    }
    return -1;
}

}
