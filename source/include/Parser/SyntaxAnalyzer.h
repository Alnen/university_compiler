#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "Grammar.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "ControlTable.h"
#include <vector>
#include <iomanip>

#include "boost/any.hpp"

#include <iosfwd>
#include <stack>

namespace Parser
{

class StackItem
{
public:
    enum class Type
    {
        TOKEN = 0,
        NONTERMINAL,
        INHERIT,
        SYNTHESIZE
    };

    StackItem(Type type): m_type(type) {}
    Type type() const { return m_type; }
    virtual ~StackItem() {}

protected:
    Type    m_type;
};

template <class TokenType>
class TokenItem : public StackItem
{
public:
    TokenItem() : StackItem(Type::TOKEN) {}
};

class BaseStackedItem : public StackItem
{
public:
    using value = boost::any;
    using value_pointer = std::shared_ptr<value>;
    using const_value_pointer = std::shared_ptr<const value>;

    BaseStackedItem(Type type): StackItem(type) {}

    template <class T>
    void push(T&& item)
    {
        m_stack.push_back(std::make_shared<boost::any>(std::forward<T>(item)));
    }

    template <class T>
    void push(std::shared_ptr<T>&& item)
    {
        m_stack.push_back(std::forward<T>(item));
    }

    std::vector<const_value_pointer> m_stack;
    const_value_pointer              m_value;
};

class NonterminalItems : public BaseStackedItem
{
public:
    NonterminalItems(): BaseStackedItem(Type::NONTERMINAL) {}
};

class BaseActionItem : public BaseStackedItem
{
public:
     BaseActionItem(Type type): BaseStackedItem(type) {}

     virtual void operator()() = 0;
};

class BaseInheritItem : public BaseActionItem
{
public:
     BaseInheritItem(): BaseActionItem(Type::INHERIT) {}

     virtual void operator()()
     {

     }
};

class BaseSynthesizeItem : public BaseActionItem
{
public:
     BaseSynthesizeItem(): BaseActionItem(Type::SYNTHESIZE) {}

     virtual void operator()()
     {

     }
};


template<class _TerminalType, class _NonterminalType, class _ActionVector>
class SyntaxAnalyzer
{
public:
    using TeminalType    = typename Grammar<_TerminalType, _NonterminalType>::TerminalType;
    using NonteminalType = typename Grammar<_TerminalType, _NonterminalType>::NonterminalType;
    using TokenType      = typename Grammar<_TerminalType, _NonterminalType>::TokenType;
    using TokenList      = typename Grammar<_TerminalType, _NonterminalType>::TokenList;
    using ActionVector   = _ActionVector;
    using Rule           = Rule<TeminalType, NonteminalType>;

    SyntaxAnalyzer(const Grammar<_TerminalType, _NonterminalType>& grammar);
    bool readNextToken(TokenType new_token);
    bool parse(Lexer::Lexer<TeminalType>& lexer);

private:
    int getRuleId(const Rule& rule);
	
private:
    using StackElement = std::pair<TokenType, boost::any>;
    ControlTable<_TerminalType, _NonterminalType>   m_control_table;
    Grammar<_TerminalType, _NonterminalType>        m_grammar;
    std::vector<TokenType>                          m_stack;
    std::vector<boost::any>                         m_valueStack;
    std::vector<boost::any>                         m_backValueStack;
};

template<class TerminalType, class NonterminalType, class ActionVector>
bool SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::parse(Lexer::Lexer<TeminalType>& lexer)
{
    m_stack.push_back(TerminalType::ENDOFFILE);
    m_stack.push_back(m_grammar.start_symbol);

    std::unique_ptr<Lexer::Token<TerminalType>> token;
    while (!m_stack.empty())
    {
        token = lexer.getToken();
        std::cout << "NEW TOKEN : " << std::setw(3) << token->type() << "|" << boost::any_cast<std::string>(token->value()) << std::endl;
        if (!readNextToken(token->type()))
        {
            return false;
        }
    }
    return true;
}

template<class TerminalType, class NonterminalType, class ActionVector>
SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::SyntaxAnalyzer(const Grammar<TerminalType, NonterminalType>& grammar) : m_grammar(grammar), m_control_table(grammar)
{
}

template<class TerminalType, class NonterminalType, class ActionVector>
bool SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::readNextToken(TokenType new_token)
{
    static bool flag = false;
    while (true)
    {
        std::cout << "stack : [";
        std::copy(m_stack.begin(), m_stack.end(), std::ostream_iterator<TokenType>(std::cout, " "));
        std::cout << "]" << std::endl;

        while (Grammar<TerminalType, NonterminalType>::isAction(m_stack.back()))
        {
            m_stack.pop_back();
        }

        if (m_stack.back() == new_token)
        {
            m_stack.pop_back();
            return true;
        }
        else if (m_grammar.isTerminal(m_stack.back()))
        {
            std::cout << "BOOM 2" << std::endl;
            return false;
        }
        else
        {
            const Rule& ct_rule = m_control_table.getRule(m_stack.back(), new_token);
            if (ct_rule.left() == Rule::EMPTY_RULE) {
                std::cout << "BOOM 1 " << m_stack.back() << "|" << new_token << std::endl;
                return false;
            }
            else
            {
                m_stack.pop_back();
                if (!(ct_rule.right().size() == 2 && ct_rule.right()[0] == NonterminalType::EPSILON))
                {
                    std::copy(ct_rule.right().rbegin(), ct_rule.right().rend(), std::back_inserter(m_stack));
                }
            }
        }
    }
}

template<class TerminalType, class NonterminalType, class ActionVector>
int SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::getRuleId(const Rule& rule)
{
    for (int i = 0; i < m_grammar.rules.size(); ++i) {
        if (m_grammar.rules[i] == rule) return i;
    }
    return -1;
}

}

#endif // SYNTAX_ANALYZER_H
