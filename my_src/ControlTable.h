#pragma once

#include "grammar.h"

namespace Grammar
{

class ControlTable
{
public:
	ControlTable(const Grammar& grammar);
	void add(size_t row_value, size_t col_value, Rule value);
	Rule getRule(size_t row_value, size_t col_value) const;
	Rule getRuleByIndex(size_t row_index, size_t col_index) const;

public:
	std::vector<int> row_names;
	std::vector<int> column_names;
	std::vector<Rule> values;
};

} // namespace Grammar