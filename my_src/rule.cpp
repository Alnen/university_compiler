#include "rule.h"

using namespace Grammar;

void TokenType::setValues(std::string new_name, int new_value)
{
	name = new_name;
	value = new_value;
}

bool TokenType::operator ==(const TokenType& other) const
{
	return (value == other.value);
}

bool TokenType::operator !=(const TokenType& other) const
{
	return (value != other.value);
}

bool TokenType::operator <(const TokenType& other) const
{
	return (value < other.value);
}

bool Rule::operator ==(const Rule& other) const
{
	if ((left == other.left) && (right.size() == other.right.size())) {
		for (int i = 0; i < right.size(); ++i) {
			if (right[i] != other.right[i]) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool Rule::operator !=(const Rule& other) const
{
	return (!(*this == other));
}

bool Rule::operator <(const Rule& other) const
{
	if (left < other.left)
		return true;
	size_t min_size = ((other.right.size() < right.size()) ? other.right.size() : right.size());
	for (int i = 0; i < min_size; ++i) {
		if (right[i] < other.right[i])
			return true;
	}
	return false;
}