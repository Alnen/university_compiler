#ifndef RULE_H
#define RULE_H

#include <vector>
#include <string>

#include "RuleHandler.h"
#include "Grammar.h"

namespace Parser
{

template<class _TerminalType, class _NonterminalType>
class Rule
{
public:
    using TerminalType = _TerminalType;
    using NonterminalType = _NonterminalType;
    using TokenType = typename std::common_type<TerminalType, NonterminalType>::type;
    static const TokenType EMPTY_RULE = -1;

    TokenType left() const { return m_left; }
    const std::vector<TokenType>& right() const { return m_right; }

	bool operator ==(const Rule& other) const;
	bool operator !=(const Rule& other) const;
	bool operator <(const Rule& other) const;

    Rule() : m_left(EMPTY_RULE) {};
    Rule(TokenType left_part, const std::vector<TokenType>& right_part, TokenType sinthesizeAction = NonterminalType::DEFAULT_SYNTHESIZE):
        m_left(left_part),
        m_right(right_part)
    {
        m_right.push_back(sinthesizeAction);
    }

protected:
    TokenType               m_left;
    std::vector<TokenType>  m_right;
};

template<class TerminalType, class NonterminalType>
bool Rule<TerminalType, NonterminalType>::operator ==(const Rule& other) const
{
    if ((m_left == other.m_left) && (m_right.size() == other.m_right.size())) {
        for (int i = 0; i < m_right.size(); ++i) {
            if (m_right[i] != other.right[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

template<class TerminalType, class NonterminalType>
bool Rule<TerminalType, NonterminalType>::operator !=(const Rule& other) const
{
    return (!(*this == other));
}

template<class TerminalType, class NonterminalType>
bool Rule<TerminalType, NonterminalType>::operator <(const Rule& other) const
{
    return (m_left < other.m_left || m_right < other.m_right);
}

} // namespace Grammar

#endif // RULE_H
