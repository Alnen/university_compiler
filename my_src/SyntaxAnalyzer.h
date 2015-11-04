#pragma once

#include "grammar.h"
#include "ControlTable.h"
#include <vector>

namespace Grammar
{

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer(const Grammar& grammar);
	int readNextToken(TokenType new_token);
	int readTokens(TokenList tokens);

private:
	int getRuleId(const Rule& rule);
	
private:
	ControlTable m_control_table;
	Grammar m_grammar;
	TokenList m_stack;
	std::vector<int> output_tree;
};

}