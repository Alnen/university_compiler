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
#include <fstream>

namespace Parser
{

std::ofstream parse_log("parse_log.txt", std::ofstream::out&std::ofstream::trunc);

template <class Nonterminal>
class NonterminalInfo
{
public:
    NonterminalInfo(Nonterminal symbol): m_symbol(symbol) {}
    boost::any& value(){ return m_value; }
    Nonterminal symbol() const { return m_symbol; }


protected:
    Nonterminal m_symbol;
    boost::any  m_value;
};

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
    bool readNextToken(std::unique_ptr<Lexer::Token<TerminalType>> new_token);
    std::pair<std::shared_ptr<boost::container::flat_map<std::string, boost::any>>, bool> parse(Lexer::Lexer<TerminalType>& lexer);
    void print(std::ostream& out);

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
void SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::print(std::ostream& out)
{
    m_control_table.print(out);
}

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
        auto nonterminalItem = std::make_unique<NonterminalItem>();
        (*(nonterminalItem->getValue()))["term"] = NonterminalInfo<NonterminalType>((NonterminalType)token);
        m_valueStack.push_back( std::move(nonterminalItem) );
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
std::pair<std::shared_ptr<boost::container::flat_map<std::string, boost::any>>, bool>
SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::parse(Lexer::Lexer<TerminalType>& lexer)
{
    pushTokenBack(NonterminalType::DEFAULT_SYNTHESIZE);
    pushTokenBack(TerminalType::ENDOFFILE);
    pushTokenBack(m_grammar.start_symbol);

    std::unique_ptr<Lexer::Token<TerminalType>> token;
    Lexer::Token<TerminalType> backup_token;
    while (!m_stack.empty())
    {
        token = lexer.getToken();
        auto token_type = token->type();
        backup_token = *token;
        parse_log << "NEW TOKEN : " << std::setw(3) << token->type() << "|" << boost::any_cast<std::string>(token->value()) << std::endl;
        if (token->type() == TerminalType::PASS) continue;
        if (!readNextToken(std::move(token)))
        {
            break;
        }
        if (token_type == TerminalType::ENDOFFILE)
        {
            break;
        }
    }
    if (m_stack.size() == 1)
    {
        parse_log << "SUCCESS";
        static_cast<BaseSynthesizeItem*>(m_valueStack[0].get())->executeHandler();
        return std::make_pair(static_cast<BaseSynthesizeItem*>(m_valueStack[0].get())->getValue(), true);
    }
    else
    {
        parse_log << "FAILURE";
        auto token_shared_ptr = std::shared_ptr<Lexer::Token<TerminalType>>(new Lexer::Token<TerminalType>(backup_token));
        auto temp_map =  boost::container::flat_map<std::string, boost::any>{ {"term", token_shared_ptr} };
        auto token_value = std::make_shared<boost::container::flat_map<std::string, boost::any>>(temp_map);


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
        //
        while(m_stack.size() != 1)
        {
            if (Grammar::isAction(m_stack.back()))
            {
                auto action_on_back = static_cast<BaseStackedItem*>(m_valueStack.back().get());
                action_on_back->executeHandler();
                if (action_on_back->type() == BaseStackedItem::Type::SYNTHESIZE)
                {
                    for (int i = m_stack.size() - 2; i >= 0; --i)
                    {
                        if (Grammar::isAction(m_stack[i]))
                        {
                            auto action_on_stack = static_cast<BaseStackedItem*>(m_valueStack[i].get());
                            action_on_stack->push(action_on_back->getValue());
                            if (action_on_stack->type() == BaseStackedItem::Type::SYNTHESIZE)
                            {
                                break;
                            }
                        }
                    }
                }
            }
            popTokenBack();
        }
        static_cast<BaseSynthesizeItem*>(m_valueStack[0].get())->executeHandler();
        return std::make_pair(static_cast<BaseSynthesizeItem*>(m_valueStack[0].get())->getValue(), false);
    }
}

template<class TerminalType, class NonterminalType, class ActionVector>
bool SyntaxAnalyzer<TerminalType, NonterminalType, ActionVector>::readNextToken(std::unique_ptr<Lexer::Token<TerminalType>> new_token)
{
    static bool flag = false;
    while (true)
    {
        parse_log << "stack : [";
        for (auto token : m_stack)
        {
            if (Grammar::isTerminal(token))
            {
                parse_log << tokenTypeMapping()[(TerminalType)token];
            }
            else if (Grammar::isNonterminal(token))
            {
                parse_log << getNonterminalTypeMapping()[(NonterminalType)token];
            }
            else
            {
                parse_log << token - 200 + 117;
            }
            parse_log << " ";
        }
        parse_log << "]" << std::endl;

        if (Grammar::isAction(m_stack.back())) // TODO
        {
            auto action_on_back = static_cast<BaseStackedItem*>(m_valueStack.back().get());
            action_on_back->executeHandler();
            if (action_on_back->type() == BaseStackedItem::Type::SYNTHESIZE)
            {
                for (int i = m_stack.size() - 2; i >= 0; --i)
                {
                    if (Grammar::isAction(m_stack[i]))
                    {
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
                auto next_nonterminal = m_valueStack[m_valueStack.size()-2].get();
                auto* next_nonterminal_map = next_nonterminal->getValue().get();
                auto* action_on_back_map = action_on_back->getValue().get();
                for (auto pair : *action_on_back_map)
                {
                    (*next_nonterminal_map)[pair.first] = pair.second;
                }
            }

            popTokenBack();
            continue;
        }
        else if (m_stack.back() == new_token->type())
        {
            popTokenBack();
            auto token_shared_ptr = std::shared_ptr<Lexer::Token<TerminalType>>(std::move(new_token));
            auto token_map = boost::container::flat_map<std::string, boost::any>{ {"term", token_shared_ptr} };
            auto token_value = std::make_shared<boost::container::flat_map<std::string, boost::any>>(token_map);
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
            parse_log << "ERROR: expected token with token_type "
                      << tokenTypeMapping()[(TerminalType)m_stack.back()] << ", but got "
                      << tokenTypeMapping()[(TerminalType)new_token->type()] << std::endl;
            return false;
        }
        else
        {
            const Rule& ct_rule = m_control_table.getRule(m_stack.back(), new_token->type());
            if (ct_rule.left() == Rule::EMPTY_RULE) {
                return false;
            }
            else
            {
                auto value = m_valueStack.back()->getValue();
                popTokenBack();
                //if (!(ct_rule.right().size() == 2 && ct_rule.right()[0] == NonterminalType::EPSILON))
                //{
                    for (int i = ct_rule.right().size() - 1; i >= 0; --i)
                    {
                        if (ct_rule.right()[i] == NonterminalType::EPSILON)
                        {
                            continue;
                        }
                        pushTokenBack(ct_rule.right()[i]);
                        if (Grammar::isAction(m_stack.back()))
                        {
                            auto action_on_stack = static_cast<BaseStackedItem*>(m_valueStack.back().get());
                            action_on_stack->push(value);
                        }
                    }
                //}
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
