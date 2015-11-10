#ifndef CONTROLTABLE_H
#define CONTROLTABLE_H

#include "Grammar.h"

#include <algorithm>
#include <iterator>
#include <iostream>

#include "Utility.h"

namespace Parser
{

template<class _TerminalType, class _NonterminalType>
class ControlTable
{
    using TeminalType    = typename Grammar<_TerminalType, _NonterminalType>::TerminalType;
    using NonteminalType = typename Grammar<_TerminalType, _NonterminalType>::NonterminalType;
    using TokenType      = typename Grammar<_TerminalType, _NonterminalType>::TokenType;
    using Rule           = Rule<TeminalType, NonteminalType>;

public:
    ControlTable(const Grammar<_TerminalType, _NonterminalType>& grammar);
    void add(size_t row_value, size_t col_value, Rule value);
    const Rule& getRule(size_t row_value, size_t col_value) const;
    const Rule& getRuleByIndex(size_t row_index, size_t col_index) const;

public:
	std::vector<int> row_names;
	std::vector<int> column_names;
    std::vector<Rule> values;

    const Rule empty_rule;
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
    values.resize(row_names.size()*column_names.size());

    for (size_t i = 0; i < grammar.rules.size(); ++i) {
        for (size_t row = 0; row < row_names.size(); ++row) {
            if (row_names[row] == grammar.rules[i].left()) {
                Set<TokenType> f_set = grammar.ruleFirst(grammar.rules[i].right());
                if (row_names[row] == 186) std::cout << "*||* 1 << " << grammar.rules[i].right() << std::endl;
                for (size_t j = 0; j < f_set.size(); ++j) {
                    if (row_names[row] == 186) std::cout << "*||* 6 << " << f_set[j] << std::endl;
                    if (f_set[j] != NonterminalType::EPSILON) {
                        if (row_names[row] == 186) std::cout << "*||* 2 << " << f_set[j] << std::endl;
                        add(row_names[row], f_set[j], grammar.rules[i]);
                    }
                    else
                    {
                        f_set = grammar.follow(TokenType(row_names[row])); // action
                        if (row_names[row] == 186) std::cout << "*||* 3 << " << std::endl;
                        for (size_t j = 0; j < f_set.size(); ++j) {
                            if (row_names[row] == 186) std::cout << "*||* 4 << " << std::endl;
                            add(row_names[row], f_set[j], grammar.rules[i]);
                        }
                        if (f_set.contain(TokenType(TerminalType::ENDOFFILE))) {
                            add(row_names[row], TerminalType::ENDOFFILE, grammar.rules[i]);
                        }
                    }
                }
                break;
            }
        }
    }
}

template<class TerminalType, class NonterminalType>
void ControlTable<TerminalType, NonterminalType>::add(size_t row_value, size_t col_value, Rule value)
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
const typename ControlTable<TerminalType, NonterminalType>::Rule&
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
const typename ControlTable<TerminalType, NonterminalType>::Rule&
ControlTable<TerminalType, NonterminalType>::getRuleByIndex(size_t row_index, size_t col_index) const
{
    if (row_index*column_names.size() + col_index >= values.size())
        return empty_rule;
    return values[row_index*column_names.size() + col_index];
}

} // namespace Grammar

#endif // CONTROLTABLE_H
