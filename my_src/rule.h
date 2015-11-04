#pragma once
#include <vector>

namespace Grammar
{

struct TokenType
{
	std::string name;
	int value;

	TokenType(int t_value = 0) : name(""), value(t_value) {};
	TokenType(std::string t_name, int t_value) : name(t_name), value(t_value) {};
	void setValues(std::string new_name, int new_value);

	bool operator ==(const TokenType& other) const;
	bool operator !=(const TokenType& other) const;
	bool operator <(const TokenType& other) const;
};

struct Rule
{
	TokenType left;
	std::vector<TokenType> right;

	bool operator ==(const Rule& other) const;
	bool operator !=(const Rule& other) const;
	bool operator <(const Rule& other) const;

	Rule() : left(-1) {};
	Rule(TokenType left_part, const std::vector<TokenType>& right_part) : left(left_part), right(right_part) {};
};

} // namespace Grammar