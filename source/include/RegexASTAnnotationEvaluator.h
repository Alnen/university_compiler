#ifndef REGEXASTANNOTATIONEVALUATOR_H
#define REGEXASTANNOTATIONEVALUATOR_H

#include <algorithm>
#include <iosfwd>

#include "Map.h"
#include "RegexASTNode.h"
#include "Utility.h"

namespace Lexer {

enum class InputCheckerType
{
    CHAR_CHECK,
    CHAR_CLASS_CHECK
};

class IInputChecker
{
public:
    IInputChecker(InputCheckerType type): m_type(type)
    {

    }

    virtual bool operator()(char input) const = 0;

    virtual bool operator==(const IInputChecker& inputChecker) const
    {
        return m_type == inputChecker.m_type;
    }

    virtual void print(std::ostream& out) const = 0;

    virtual ~IInputChecker()
    {

    }

protected:
    InputCheckerType m_type;
};

std::ostream& operator << (std::ostream& out, IInputChecker* check);

class CharChecker : public IInputChecker
{
public:
    CharChecker(char check): IInputChecker(InputCheckerType::CHAR_CHECK), m_check(check)
    {
    }

    virtual bool operator()(char input) const override
    {
        return input == m_check;
    }

    virtual bool operator==(const IInputChecker& inputChecker) const override
    {
        if ((this)->IInputChecker::operator ==(inputChecker))
        {
            return m_check == static_cast<const CharChecker&>(inputChecker).m_check;
        }
        else
        {
            return false;
        }
    }

    virtual void print(std::ostream& out) const override
    {
        out << m_check;
    }

    virtual ~CharChecker()
    {

    }

protected:
    char m_check;
};

class CharClassChecker : public IInputChecker
{
public:
    CharClassChecker(std::vector<char>& charChecks, std::vector<std::pair<char, char>>& charRangeChecks):
        IInputChecker(InputCheckerType::CHAR_CLASS_CHECK),
        m_charChecks(charChecks),
        m_charRangeChecks(charRangeChecks)
    {
        std::sort(charChecks.begin(), charChecks.begin());
        std::sort(charRangeChecks.begin(), charRangeChecks.begin(), [](auto range1, auto range2){ return range1.first < range2.first; });
    }

    virtual bool operator()(char input) const override
    {
        return  std::find(m_charChecks.begin(), m_charChecks.end(), input) != m_charChecks.end() ||
                std::find_if(m_charRangeChecks.begin(), m_charRangeChecks.end(), [input, this](auto range){ return range.first >= input && range.second <= input; }) != m_charRangeChecks.end();
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

namespace RegexAST {

class RegexASTAnnotationEvaluator
{
public:
    using answer_t = std::pair<Map<std::shared_ptr<IInputChecker>, std::vector<BasicLeaf::LeafId>, PointerEquality>,
                            std::vector<std::vector<BasicLeaf::LeafId>>>;

    answer_t evaluate(BasicNode* ast, BasicLeaf::LeafId first_leaf_id)
    {
        m_numberOfLeaves = 0;
        m_followpos_table.clear();
        m_inputToIdMapping.clear();
        m_firstLeafId = first_leaf_id;

        evaluate_impl(ast);
        return std::make_pair(std::move(m_inputToIdMapping), std::move(m_followpos_table));
    }

protected:
    void evaluate_impl(BasicNode* ast);

    Map<std::shared_ptr<IInputChecker>, std::vector<BasicLeaf::LeafId>, PointerEquality> m_inputToIdMapping;
    std::vector<std::vector<BasicLeaf::LeafId>> m_followpos_table;
    BasicLeaf::LeafId m_firstLeafId;
    size_t m_numberOfLeaves = 0;
};

}
}

#endif // REGEXASTANNOTATIONEVALUATOR_H
