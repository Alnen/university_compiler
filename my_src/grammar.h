#pragma once
#include "set.h"
#include "rule.h"
#include "GrammarSymbols.h"

namespace Grammar
{

using TokenSet = Set<TokenType>;
using TokenList = std::vector<TokenType>;
using RuleList = std::vector<Rule>;

class Grammar
{
public:
	TokenSet terminals;
	TokenSet nonterminals;
	RuleList rules;
	TokenType start_symbol;

private:
	TokenSet readSetFromFile(const char* file_name, size_t start_index = 0);
	RuleList rulesUnion(const RuleList& rules1, const RuleList& rules2) const;
	int getTermId(std::string term_name, const TokenSet& term_set) const;
	int getTermId(std::string term_name, const TokenSet& term_set, int new_termimal) const;
	int getTermId(std::string term_name, const TokenSet& terminal_set, const TokenSet& nonterminal_set, int new_termimal = 0) const;
	TokenSet first(TokenType A, TokenSet& checked_rules) const;
	TokenSet follow(TokenType A, TokenSet& checked_rules) const;

public:
	Grammar();
	Grammar(const TokenSet& set_terminals, const RuleList& set_rules);
	void readTerminals(const char* file_name, int flag = 0);
	void writeTerminals(const char* file_name, int flag = 0) const;
	void readNonterminals(const char* file_name, int flag = 0);
	void writeNonterminals(const char* file_name, int flag = 0) const;
	void readRules(const char* file_name, int write_nonterminals = 1, int flag = 0);
	void writeRules(const char* file_name, int flag = 0) const;
	bool isTerminal(TokenType symbol) const;
	bool isNonterminal(TokenType symbol) const;
	int getTermId(std::string term_name, int new_termimal = 0) const;
	TokenSet ruleFirst(TokenList A) const;
	TokenSet first(TokenType A) const;
	TokenSet follow(TokenType A) const;
	bool isLL1() const;
};

} // namespace Grammar