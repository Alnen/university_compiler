#pragma once

#include <vector>
#include <string>

namespace Parser
{

template <class TokenType>
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

template <class TokenType>
bool Rule<TokenType>::operator ==(const Rule& other) const
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

template <class TokenType>
bool Rule<TokenType>::operator !=(const Rule& other) const
{
    return (!(*this == other));
}

template <class TokenType>
bool Rule<TokenType>::operator <(const Rule& other) const
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

} // namespace Grammar
