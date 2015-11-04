#include "RegexAST/RegexASTParser.h"
#include "StdFix.h"

namespace Lexer {
namespace RegexAST {

bool RegexToASTParser::parse_char_sequence_element(const std::string& regex, CharClassCheckLeaf& node)
{
    if (m_offset+1 < regex.size() && regex[m_offset] == '\\')
    {
        node.addChar(regex[m_offset + 1]);
        m_offset += 2;
        return true;
    }
    else if (m_offset+2 < regex.size() && regex[m_offset+1] == '-')
    {
        node.addCharRange(regex[m_offset], regex[m_offset+2]);
        m_offset += 3;
        return true;
    }
    else if (m_offset < regex.size() && regex[m_offset] == ']')
    {
        return false;
    }
    else if (m_offset < regex.size())
    {
        node.addChar(regex[m_offset]);
        m_offset += 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool RegexToASTParser::parse_char_sequence(const std::string& regex, CharClassCheckLeaf& node)
{
    if (!parse_char_sequence_element(regex, node))
    {
        return false;
    }

    while (parse_char_sequence_element(regex, node));

    return true;
}

RegexToASTParser::parse_asnswer_t RegexToASTParser::parse_char_class(const std::string& regex)
{
    if(m_offset < regex.size() && regex[m_offset] == '[')
    {
        ++m_offset;
    }
    else
    {
        return parse_asnswer_t();
    }

    std::unique_ptr<CharClassCheckLeaf> char_class = std::make_unique<CharClassCheckLeaf>( ++m_idGenerator );
    if (!parse_char_sequence(regex, *char_class.get()))
    {
        return parse_asnswer_t();
    }

    if(m_offset < regex.size() && regex[m_offset] == ']')
    {
        ++m_offset;
        return parse_asnswer_t(true, std::move(char_class));
    }
    else
    {
        return parse_asnswer_t();
    }
}

RegexToASTParser::parse_asnswer_t RegexToASTParser::parse_char(const std::string& regex)
{

    if (m_offset < regex.size())
    {
        parse_asnswer_t answer(true, std::make_unique<CharCheckLeaf>(++m_idGenerator, regex[m_offset]));
        m_offset += 1;
        return answer;
    }
    else
    {
        return parse_asnswer_t();
    }
}

RegexToASTParser::parse_asnswer_t RegexToASTParser::parse_group(const std::string& regex)
{

    if(m_offset < regex.size() && regex[m_offset] == '(')
    {
        ++m_offset;
    }
    else
    {
        return parse_asnswer_t();
    }

    parse_asnswer_t current_ast_base = std::move(parse_or(regex));
    if (!current_ast_base.m_success)
    {
        return current_ast_base;
    }

    if(m_offset < regex.size() && regex[m_offset] == ')')
    {
        m_offset += 1;
        return current_ast_base;
    }
    else
    {
        return parse_asnswer_t();
    }
}

RegexToASTParser::parse_asnswer_t RegexToASTParser::parse_single_element(const std::string& regex)
{

    parse_asnswer_t current_ast_base;
    if (m_offset < regex.size())
    {
        switch (regex[m_offset])
        {
            case '(':
                current_ast_base = parse_group(regex);
                break;

            case ')':
                current_ast_base = parse_asnswer_t();
                break;

            case '[':
                current_ast_base = parse_char_class(regex);
                break;

            case ']':
                current_ast_base = parse_asnswer_t();
                break;

            case '|':
                current_ast_base = parse_asnswer_t();
                break;

            case '\\':
                ++m_offset;
                current_ast_base = parse_char(regex);
                break;

            default:
                current_ast_base = parse_char(regex);
                break;
        }
    }
    else
    {
        current_ast_base = parse_asnswer_t();
    }
    return current_ast_base;
}

RegexToASTParser::parse_asnswer_t RegexToASTParser::parse_zero_or_more(const std::string& regex)
{

    parse_asnswer_t current_ast_base = std::move(parse_single_element(regex));
    if(!current_ast_base.m_success)
    {
        return current_ast_base;
    }

    if(m_offset < regex.size() && regex[m_offset] == '*')
    {
        current_ast_base.m_ast = std::move(std::make_unique<StarNode>(std::move(current_ast_base.m_ast)));
        ++m_offset;
    }

    return current_ast_base;
}

RegexToASTParser::parse_asnswer_t RegexToASTParser::parse_sequence(const std::string& regex)
{

    parse_asnswer_t current_ast_base = std::move(parse_zero_or_more(regex));
    if(!current_ast_base.m_success)
    {
        return current_ast_base;
    }

    parse_asnswer_t current_node;
    while(true)
    {
        current_node = std::move(parse_zero_or_more(regex));

        // combine answers
        if(current_node.m_success)
        {
            current_ast_base.m_ast = std::make_unique<CatNode>(std::move(current_ast_base.m_ast), std::move(current_node.m_ast));
        }
        else
        {
            break;
        }
    }
    return current_ast_base;
}

RegexToASTParser::parse_asnswer_t RegexToASTParser::parse_or(const std::string& regex)
{
    parse_asnswer_t current_ast_base = std::move(parse_sequence(regex));
    if(!current_ast_base.m_success)
    {
        return current_ast_base;
    }

    while(m_offset < regex.size() && regex[m_offset] == '|')
    {
        ++m_offset;
        parse_asnswer_t current_node = std::move(parse_sequence(regex));
        if(!current_node.m_success)
        {
            return current_node;
        }
        // combine answers
        current_ast_base.m_ast = std::make_unique<OrNode>(std::move(current_ast_base.m_ast), std::move(current_node.m_ast));
    }
    return current_ast_base;
}

RegexToASTParser::parse_asnswer_t RegexToASTParser::parse(const std::string& regex)
{
    m_offset = 0;
    return parse_or(regex);
}

}
}


