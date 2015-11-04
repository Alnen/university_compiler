#ifndef CHARCHECKER_H
#define CHARCHECKER_H

#include <vector>

#include "IInputHandler.h"

namespace Lexer {

class CharClassChecker : public IInputChecker
{
public:
    using Range = std::pair<char, char>;
    CharClassChecker(std::vector<char>& charChecks, std::vector<std::pair<char, char>>& charRangeChecks):
        IInputChecker(InputCheckerType::CHAR_CLASS_CHECK),
        m_charChecks(charChecks),
        m_charRangeChecks(charRangeChecks)
    {
        std::sort(charChecks.begin(), charChecks.begin());
        std::sort(charRangeChecks.begin(), charRangeChecks.begin(), [](Range range1, Range range2){ return range1.first < range2.first; });
    }

    virtual bool operator()(char input) const override
    {
        return  std::find(m_charChecks.begin(), m_charChecks.end(), input) != m_charChecks.end() ||
                std::find_if(m_charRangeChecks.begin(), m_charRangeChecks.end(), [input, this](Range range){ return range.first <= input && range.second >= input; }) != m_charRangeChecks.end();
    }

    virtual bool operator==(const IInputChecker& inputChecker) const override
    {
        if ((this)->IInputChecker::operator ==(inputChecker))
        {
            return  m_charChecks == static_cast<const CharClassChecker&>(inputChecker).m_charChecks &&
                    m_charRangeChecks == static_cast<const CharClassChecker&>(inputChecker).m_charRangeChecks;
        }
        else
        {
            return false;
        }
    }

    virtual bool operator<(const IInputChecker& inputChecker) const override
    {
        if ((this)->IInputChecker::operator==(inputChecker))
        {
            const CharClassChecker& rhs = static_cast<const CharClassChecker&>(inputChecker);
            if (m_charChecks.empty() && rhs.m_charChecks.empty())
            {
                return  m_charRangeChecks < rhs.m_charRangeChecks;
            }
            else
            {
                return  m_charChecks < rhs.m_charChecks ||
                        m_charRangeChecks < rhs.m_charRangeChecks;
            }
        }
        else
        {
            return (this)->IInputChecker::operator<(inputChecker);
        }
    }

    virtual void print(std::ostream& out) const override
    {
        out << "[";
        for (auto check : m_charChecks)
        {
            out << check;
        }
        for (auto range : m_charRangeChecks)
        {
            out << range.first << "-" << range.second;
        }
        out << "]";
    }

    virtual ~CharClassChecker()
    {

    }

protected:
    std::vector<char> m_charChecks;
    std::vector<std::pair<char, char>> m_charRangeChecks;
};

}

#endif // CHARCHECKER_H
