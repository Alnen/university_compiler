#include "ControlTable.h"

Grammar::ControlTable::ControlTable(const Grammar& grammar)
{
	for (size_t i = 0; i < grammar.nonterminals.size(); ++i) {
		row_names.push_back(grammar.nonterminals[i].value);
	}
	for (size_t i = 0; i < grammar.terminals.size(); ++i) {
		column_names.push_back(grammar.terminals[i].value);
	}
	column_names.push_back(FINAL_STATE_SYMBOL);
	for (size_t i = 0; i < row_names.size(); ++i) {
		for (size_t j = 0; j < column_names.size(); ++j) {
			values.push_back(Rule());
		}
	}
	for (size_t i = 0; i < grammar.rules.size(); ++i) {
		for (size_t row = 0; row < row_names.size(); ++row) {
			if (row_names[row] == grammar.rules[i].left.value) {
				TokenSet f_set = grammar.ruleFirst(grammar.rules[i].right);
				for (size_t j = 0; j < f_set.size(); ++j) {
					if (f_set[j].value != EPSILON) {
						add(row_names[row], f_set[j].value, grammar.rules[i]);
					}
				}
				if (f_set.contain(TokenType(EPSILON))) {
					f_set = grammar.follow(TokenType(row_names[row]));
					for (size_t j = 0; j < f_set.size(); ++j) {
						add(row_names[row], f_set[j].value, grammar.rules[i]);
					}
					if (f_set.contain(TokenType(FINAL_STATE_SYMBOL))) {
						add(row_names[row], FINAL_STATE_SYMBOL, grammar.rules[i]);
					}
				}
				break;
			}
		}
	}
}

void Grammar::ControlTable::add(size_t row_value, size_t col_value, Rule value)
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

Grammar::Rule Grammar::ControlTable::getRule(size_t row_value, size_t col_value) const
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

Grammar::Rule Grammar::ControlTable::getRuleByIndex(size_t row_index, size_t col_index) const
{
	if (row_index*column_names.size() + col_index >= values.size())
		return Rule();
	return values[row_index*column_names.size() + col_index];
}