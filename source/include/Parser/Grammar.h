#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <utility>

#include "set.h"
#include "Rule.h"



namespace Parser
{


template<class _TerminalType, class _NonterminalType>
class Grammar
{
public:
    using TerminalType = _TerminalType;
    using NonterminalType = _NonterminalType;
    using TokenType = typename std::common_type<TerminalType, NonterminalType>::type;
    using TokenSet = Set<TokenType>;
    using TokenList = std::vector<TokenType>;
    using RuleList = std::vector<Rule<TokenType>>;

    Grammar() = delete;
    Grammar(const RuleList& set_rules, NonterminalType start_symbol);
    bool isTerminal(TokenType symbol) const;
    bool isNonterminal(TokenType symbol) const;
    TokenSet ruleFirst(TokenList A) const;
    TokenSet first(TokenType A) const;
    TokenSet follow(TokenType A) const;
    bool isLL1() const;

	TokenSet terminals;
	TokenSet nonterminals;
	RuleList rules;
    TokenType start_symbol;

private:
    TokenSet first(TokenType A, TokenSet& checked_rules) const;
    TokenSet follow(TokenType A, TokenSet& checked_rules) const;
};

template <class Terminal, class Nonterminal>
Grammar<Terminal, Nonterminal>::Grammar(const RuleList& set_rules, Nonterminal _start_symbol)
{
    for (size_t i = Terminal::TERMINAL_START_INDEX + 1; i < Terminal::TERMINAL_FINISH_INDEX; ++i)
    {
        terminals.add(Terminal(i)); // TODO
    }
    for (size_t i = Nonterminal::NONTERMINAL_START_INDEX + 1; i < Nonterminal::NONTERMINAL_FINISH_INDEX; ++i)
    {
        nonterminals.add(Nonterminal(i));
    }
    rules =  set_rules;
    start_symbol = _start_symbol;
}

template <class Terminal, class Nonterminal>
bool Grammar<Terminal, Nonterminal>::isTerminal(TokenType symbol) const
{
    for (size_t i = 0; i < terminals.size(); ++i) {
        if (terminals[i] == symbol)
            return true;
    }
    return false;
}

template <class Terminal, class Nonterminal>
bool Grammar<Terminal, Nonterminal>::isNonterminal(TokenType symbol) const
{
    for (size_t i = 0; i < nonterminals.size(); ++i) {
        if (nonterminals[i] == symbol)
            return true;
    }
    return false;
}

template <class Terminal, class Nonterminal>
typename Grammar<Terminal, Nonterminal>::TokenSet
Grammar<Terminal, Nonterminal>::first(TokenType A, TokenSet& checked_rules) const
{
    TokenSet first_set;
    if ((isTerminal(A)) || (A == Nonterminal::EPSILON)) {
        first_set.add(A);
        return first_set;
    }
    checked_rules.add(A);
    for (size_t i = 0; i < rules.size(); ++i) {
        if (rules[i].left() == A) {
            if (rules[i].right()[0] == Nonterminal::EPSILON) {
                first_set.add(TokenType(Nonterminal::EPSILON));
            }
            else {
                for (size_t j = 0; j < rules[i].right().size(); ++j) {
                    if (!checked_rules.contain(rules[i].right()[j])) {
                        TokenSet f_set = first(rules[i].right()[j], checked_rules);
                        if (f_set.size() == 0) continue;
                        if (f_set.contain(TokenType(Nonterminal::EPSILON))) {
                            for (size_t k = 0; k < f_set.size(); ++k) {
                                if ((f_set[k] != Nonterminal::EPSILON) || (i == (rules[i].right().size() - 1))) {
                                    first_set.add(f_set[k]);
                                }
                            }
                        }
                        else {
                            first_set.update(f_set);
                            break;
                        }
                    }
                }
            }
        }
    }
    return first_set;
}

template <class Terminal, class Nonterminal>
typename Grammar<Terminal, Nonterminal>::TokenSet
Grammar<Terminal, Nonterminal>::ruleFirst(std::vector<TokenType> A) const
{
    TokenSet first_set;
    TokenSet checked_rules;
    if (A.size() == 0)
        return first_set;
    if (A[0] == Nonterminal::EPSILON) {
        first_set.add(TokenType(Nonterminal::EPSILON));
    }
    else {
        for (size_t i = 0; i < A.size(); ++i) {
            if (!checked_rules.contain(A[i])) {
                TokenSet f_set = first(A[i], checked_rules);
                if (f_set.size() == 0) continue;
                if (f_set.contain(TokenType(Nonterminal::EPSILON))) {
                    for (size_t k = 0; k < f_set.size(); ++k) {
                        if ((f_set[k] != Nonterminal::EPSILON) || (i == (A.size() - 1))) {
                            first_set.add(f_set[k]);
                        }
                    }
                }
                else {
                    first_set.update(f_set);
                    break;
                }
            }
        }
    }
    return first_set;
}

template <class Terminal, class Nonterminal>
typename Grammar<Terminal, Nonterminal>::TokenSet
Grammar<Terminal, Nonterminal>::first(TokenType A) const
{
    TokenSet checked_rules;
    return first(A, checked_rules);
}

template <class Terminal, class Nonterminal>
typename Grammar<Terminal, Nonterminal>::TokenSet
Grammar<Terminal, Nonterminal>::follow(TokenType A, TokenSet& checked_rules) const
{
    TokenSet follow_set;
    checked_rules.add(A);
    if (A == start_symbol) {
        follow_set.add(Terminal::ENDOFFILE);
    }
    for (size_t i = 0; i < rules.size(); ++i) {
        for (size_t j = 0; j < rules[i].right().size(); ++j) {
            if (rules[i].right()[j] == A) {
                if (j == (rules[i].right().size() - 1)) {
                    if (!checked_rules.contain(rules[i].left())) {
                        TokenSet f_set = follow(rules[i].left(), checked_rules);
                        if (f_set.size() > 0) follow_set.update(f_set);
                    }
                }
                else
                {
                    std::vector<TokenType> rest_of_rule(rules[i].right().begin() + (j + 1), rules[i].right().end());
                    TokenSet f_set = ruleFirst(rest_of_rule);
                    if (f_set.size() == 0) throw std::runtime_error("f_set.size() == 0");
                    for (size_t k = 0; k < f_set.size(); ++k) {
                        if (f_set[k] != Nonterminal::EPSILON) {
                            follow_set.add(f_set[k]);
                        }
                    }
                    if (f_set.contain(TokenType(Nonterminal::EPSILON)) && (!checked_rules.contain(rules[i].left()))) {
                        f_set = follow(rules[i].left(), checked_rules);
                        if (f_set.size() > 0) follow_set.update(f_set);
                    }
                }
            }
        }
    }
    return follow_set;
}

template <class Terminal, class Nonterminal>
typename Grammar<Terminal, Nonterminal>::TokenSet
Grammar<Terminal, Nonterminal>::follow(TokenType A) const
{
    TokenSet checked_rules;
    return follow(A, checked_rules);
}

template <class Terminal, class Nonterminal>
bool Grammar<Terminal, Nonterminal>::isLL1() const
{
    for (size_t i = 0; i < nonterminals.size(); ++i) {
        std::vector<std::vector<TokenType>> t_rules;
        for (size_t j = 0; j < rules.size(); ++j) {
            if (nonterminals[i] == rules[j].left()) {
                t_rules.push_back(rules[j].right());
            }
        }
        for (size_t j = 0; j < t_rules.size(); ++j) {
            for (size_t k = j + 1; k < t_rules.size(); ++k) {
                TokenSet f_set = ruleFirst(t_rules[j]);
                TokenSet f_set2 = ruleFirst(t_rules[k]);
                TokenSet fset_intersection = f_set.intersection(f_set2);
                if (fset_intersection.size() > 0) {
                    return false;
                }
                if (f_set.contain(TokenType(Nonterminal::EPSILON))) {
                    TokenSet f_set3 = follow(nonterminals[i]);
                    TokenSet fset_intersection2 = f_set3.intersection(f_set2);
                    if (fset_intersection2.size() > 0) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

} // namespace Grammar

#endif // GRAMMAR_H
