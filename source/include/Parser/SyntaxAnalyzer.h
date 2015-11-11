#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "Grammar.h"
#include "ActionFactory.h"
#include "StackItem.h"
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

template<class _TerminalType, class _NonterminalType, class _ActionVector>
class SyntaxAnalyzer
{
public:
    using Grammar        = Grammar<_TerminalType, _NonterminalType>;
    using TerminalType    = typename Grammar::TerminalType;
    using NonteminalType = typename Grammar::NonterminalType;
    using TokenType      = typename Grammar::TokenType;
    using TokenList      = typename Grammar::TokenList;
    using ActionVector   = _ActionVector;
    using Rule           = Rule<TerminalType, NonteminalType>;
    using StackItemPointer = std::unique_ptr<Parser::StackItem>;

    SyntaxAnalyzer(const Grammar& grammar);
    bool readNextToken(Lexer::Token<TerminalType>* new_token);
    bool parse(Lexer::Lexer<TerminalType>& lexer);

private:
    int getRuleId(const Rule& rule);
    void pushTokenBack(TokenType token);
    void popTokenBack();
	
private:
    using StackElement = std::pair<TokenType, boost::any>;
    ControlTable<_TerminalType, _NonterminalType>   m_control_table;
    Grammar                                         m_grammar;
    std::vector<TokenType>                          m_stack;
    std::vector<StackItemPointer>                   m_valueStack;
    ActionFactory<_ActionVector, _NonterminalType>  m_actionfactory;
};

template<class TerminalType, class NonterminalType, class ActionVector>
SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::SyntaxAnalyzer(const Grammar& grammar) : m_grammar(grammar), m_control_table(grammar)
{
}

template<class TerminalType, class NonterminalType, class ActionVector>
void SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::pushTokenBack(TokenType token)
{
    m_stack.push_back(token);
    if (Grammar::isTerminal(token))
    {
        m_valueStack.emplace_back();
    }
    else if (Grammar::isNonterminal(token))
    {
        m_valueStack.push_back( std::make_unique<NonterminalItem>() );
    }
    else if (Grammar::isAction(token))
    {
        m_valueStack.push_back(m_actionfactory((NonterminalType)token));
    }
    else
    {
        std::cout << "ERROR " << token << std::endl;
        throw std::runtime_error("void SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::pushTokenBack(TokenType token): error");
    }
}

template<class TerminalType, class NonterminalType, class ActionVector>
void SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::popTokenBack()
{
    m_stack.pop_back();
    m_valueStack.pop_back();
}

template<class TerminalType, class NonterminalType, class ActionVector>
bool SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::parse(Lexer::Lexer<TerminalType>& lexer)
{
    pushTokenBack(NonterminalType::DEFAULT_SYNTHESIZE);
    pushTokenBack(TerminalType::ENDOFFILE);
    pushTokenBack(m_grammar.start_symbol);

    std::unique_ptr<Lexer::Token<TerminalType>> token;
    while (!m_stack.empty())
    {
        token = lexer.getToken();
        std::cout << "NEW TOKEN : " << std::setw(3) << token->type() << "|" << boost::any_cast<std::string>(token->value()) << std::endl;
        if (!readNextToken(token.get()))
        {
            break;
        }
        if (token->type() == TerminalType::ENDOFFILE)
        {
            break;
        }
    }
    if (m_stack.size() == 1)
    {

        return true;
    }
    else
    {
        return false;
    }
}

template<class TerminalType, class NonterminalType, class ActionVector>
bool SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::readNextToken(Lexer::Token<TerminalType>* new_token)
{
    static bool flag = false;
    while (true)
    {
        std::cout << "stack : [";
        std::copy(m_stack.begin(), m_stack.end(), std::ostream_iterator<TokenType>(std::cout, " "));
        std::cout << "]" << std::endl;

        if (Grammar::isAction(m_stack.back())) // TODO
        {
            auto action_on_back = static_cast<BaseStackedItem*>(m_valueStack.back().get());
            action_on_back->executeHandler();
            /*if (action_on_back->type() == BaseStackedItem::Type::SYNTHESIZE)
            {
                for (int i = m_stack.size() - 2; i >= 0; --i)
                {
                    if (Grammar::isAction(m_stack[i]))
                    {
                        //std::cout << i << " " << m_stack[m_stack.size() - i] << std::endl;
                        auto action_on_stack = static_cast<BaseStackedItem*>(m_valueStack[i].get());
                        action_on_stack->push(action_on_back->getValue());
                        if (action_on_stack->type() == BaseStackedItem::Type::SYNTHESIZE)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                //auto next_nonterminal = m_valueStack[m_valueStack.size()-2].get();
                //next_nonterminal->getValue() = action_on_back->getValue();
            }*/

            popTokenBack();
            continue;
        }
        else if (m_stack.back() == new_token->type())
        {
            popTokenBack();
            auto token_value = std::make_shared<boost::any>(new_token->value());
            for (int i = m_stack.size() - 1; i >= 0; --i)
            {
                if (Grammar::isAction(m_stack[i]))
                {
                    //std::cout << i << " " << m_stack[m_stack.size() - i] << std::endl;
                    auto action_on_stack = static_cast<BaseStackedItem*>(m_valueStack[i].get());
                    action_on_stack->push(token_value);
                    if (action_on_stack->type() == BaseStackedItem::Type::SYNTHESIZE)
                    {
                        break;
                    }
                }
            }
            return true;
        }
        else if (m_grammar.isTerminal(m_stack.back()))
        {
            std::cout << "BOOM 2" << std::endl;
            return false;
        }
        else
        {
            const Rule& ct_rule = m_control_table.getRule(m_stack.back(), new_token->type());
            if (ct_rule.left() == Rule::EMPTY_RULE) {
                std::cout << "BOOM 1 " << m_stack.back() << "|" << new_token << std::endl;
                return false;
            }
            else
            {
                auto value = m_valueStack.back()->getValue();
                popTokenBack();
                if (!(ct_rule.right().size() == 2 && ct_rule.right()[0] == NonterminalType::EPSILON))
                {
                    for (int i = ct_rule.right().size() - 1; i >= 0; --i)
                    {
                        pushTokenBack(ct_rule.right()[i]);
                        if (Grammar::isAction(m_stack.back()))
                        {
                            auto action_on_stack = static_cast<BaseStackedItem*>(m_valueStack.back().get());
                            action_on_stack->push(value);
                        }
                    }
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
