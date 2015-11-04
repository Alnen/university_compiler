#include "SyntaxAnalyzer.h"

Grammar::SyntaxAnalyzer::SyntaxAnalyzer(const Grammar& grammar) : m_grammar(grammar), m_control_table(grammar)
{
	m_stack.push_back(TokenType(FINAL_STATE_SYMBOL));
	m_stack.push_back(grammar.start_symbol);
}

int Grammar::SyntaxAnalyzer::readNextToken(TokenType new_token)
{
	if (m_stack[m_stack.size() - 1] == TokenType(FINAL_STATE_SYMBOL)) {
		return 0;
	}
	if (m_stack[m_stack.size() - 1] == new_token) {
		m_stack.pop_back();
		if (m_stack[m_stack.size() - 1] == TokenType(FINAL_STATE_SYMBOL)) {
			return 0;
		}
		return 1;
	}
	else if (m_grammar.isTerminal(m_stack[m_stack.size() - 1])) {
		return -1;
	}
	Rule ct_rule = m_control_table.getRule(m_stack[m_stack.size() - 1].value, new_token.value);
	if (ct_rule.left.value < 0) {
		return -1;
	}
	else {
		output_tree.push_back(getRuleId(ct_rule));
		m_stack.pop_back();
		for (int j = ct_rule.right.size() - 1; j >= 0; --j) {
			m_stack.push_back(ct_rule.right[j]);
		}
		if (m_stack[m_stack.size() - 1] == TokenType(FINAL_STATE_SYMBOL)) {
			return 0;
		}
	}
	return 1;
}

int Grammar::SyntaxAnalyzer::readTokens(TokenList tokens)
{
	size_t i = 0;
	Rule ct_rule;
	while (i < tokens.size()) {
		if (m_stack[m_stack.size() - 1] == TokenType(FINAL_STATE_SYMBOL)) {
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
		ct_rule = m_control_table.getRule(m_stack[m_stack.size() - 1].value, tokens[i].value);
		if (ct_rule.left.value < 0) {
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
	if (m_stack[m_stack.size() - 1] == TokenType(FINAL_STATE_SYMBOL)) {
		return 0;
	}
	return 1;
}

int Grammar::SyntaxAnalyzer::getRuleId(const Rule& rule)
{
	for (int i = 0; i < m_grammar.rules.size(); ++i) {
		if (m_grammar.rules[i] == rule) return i;
	}
	return -1;
}