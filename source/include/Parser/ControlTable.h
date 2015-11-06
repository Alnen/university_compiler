#ifndef CONTROLTABLE_H
#define CONTROLTABLE_H

#include "Grammar.h"

#include <algorithm>
#include <iterator>
#include <iostream>

namespace Parser
{

template<class _TerminalType, class _NonterminalType>
class ControlTable
{
    using TeminalType    = typename Grammar<_TerminalType, _NonterminalType>::TerminalType;
    using NonteminalType = typename Grammar<_TerminalType, _NonterminalType>::NonterminalType;
    using TokenType      = typename Grammar<_TerminalType, _NonterminalType>::TokenType;

public:
    ControlTable(const Grammar<_TerminalType, _NonterminalType>& grammar);
    void add(size_t row_value, size_t col_value, Rule<TokenType> value);
    const Rule<TokenType>& getRule(size_t row_value, size_t col_value) const;
    const Rule<TokenType>& getRuleByIndex(size_t row_index, size_t col_index) const;

public:
	std::vector<int> row_names;
	std::vector<int> column_names;
    std::vector<Rule<TokenType>> values;

    const Rule<TokenType> empty_rule;
};

template<class TerminalType, class NonterminalType>
ControlTable<TerminalType, NonterminalType>::ControlTable(const Grammar<TerminalType, NonterminalType>& grammar)
{
    for (size_t i = 0; i < grammar.nonterminals.size(); ++i) {
        row_names.push_back(grammar.nonterminals[i]);
    }
    for (size_t i = 0; i < grammar.terminals.size(); ++i) {
        column_names.push_back(grammar.terminals[i]);
    }
    column_names.push_back(TerminalType::ENDOFFILE);
    for (size_t i = 0; i < row_names.size(); ++i) {
        for (size_t j = 0; j < column_names.size(); ++j) {
            values.push_back(Rule<TokenType>());
        }
    }
    for (size_t i = 0; i < grammar.rules.size(); ++i) {
        for (size_t row = 0; row < row_names.size(); ++row) {
            if (row_names[row] == grammar.rules[i].left()) {
                Set<TokenType> f_set = grammar.ruleFirst(grammar.rules[i].right());
                for (size_t j = 0; j < f_set.size(); ++j) {
                    if (f_set[j] != NonterminalType::EPSILON) {
                        add(row_names[row], f_set[j], grammar.rules[i]);
                    }
                }
                if (f_set.contain(TokenType(NonterminalType::EPSILON))) {
                    f_set = grammar.follow(TokenType(row_names[row]));
                    for (size_t j = 0; j < f_set.size(); ++j) {
                        add(row_names[row], f_set[j], grammar.rules[i]);
                    }
                    if (f_set.contain(TokenType(TerminalType::ENDOFFILE))) {
                        add(row_names[row], TerminalType::ENDOFFILE, grammar.rules[i]);
                    }
                }
                break;
            }
        }
    }
}

template<class TerminalType, class NonterminalType>
void ControlTable<TerminalType, NonterminalType>::add(size_t row_value, size_t col_value, Rule<TokenType> value)
{
    size_t i = 0;
    for (; i < row_names.size(); ++i) {
        if (row_names[i] == row_value) break;
    }
    size_t j = 0;
    for (; j < column_names.size(); ++j) {
        if (column_names[j] == col_value) break;
    }
    values[i*column_names.size() + j] = value;
}

template<class TerminalType, class NonterminalType>
const Rule<typename ControlTable<TerminalType, NonterminalType>::TokenType>&
ControlTable<TerminalType, NonterminalType>::getRule(size_t row_value, size_t col_value) const
{
    size_t i = 0;
    for (; i < row_names.size(); ++i) {
        if (row_names[i] == row_value) break;
    }
    size_t j = 0;
    for (; j < column_names.size(); ++j) {
        if (column_names[j] == col_value) break;
    }
    return getRuleByIndex(i, j);
}

template<class TerminalType, class NonterminalType>
const Rule<typename ControlTable<TerminalType, NonterminalType>::TokenType>&
ControlTable<TerminalType, NonterminalType>::getRuleByIndex(size_t row_index, size_t col_index) const
{
    if (row_index*column_names.size() + col_index >= values.size())
        return empty_rule;
    return values[row_index*column_names.size() + col_index];
}

} // namespace Grammar

#endif // CONTROLTABLE_H
