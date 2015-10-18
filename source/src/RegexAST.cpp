#include "RegexAST.h"

namespace Lexer {
namespace RegexAST {

std::ostream& operator<<(std::ostream& out, element& elem)
{
    elem.print(out, 0);
    return out;
}

parse_asnswer_t parse_char_sequence_element(const std::string& regex, size_t offset)
{

    if (offset+1 < regex.size() && regex[offset] == '\\')
    {
        return parse_char(regex, offset+1);
    }
    else if (offset+2 < regex.size() && regex[offset+1] == '-')
    {
        return parse_asnswer_t(true, std::make_unique<char_range_check>(regex[offset], regex[offset+2]), offset+3);
    }
    else if (offset < regex.size() && regex[offset] == ']')
    {
        return parse_asnswer_t(offset);
    }
    else if (offset < regex.size())
    {
        return parse_char(regex, offset);
    }
    else
    {
        return parse_asnswer_t(offset);
    }
}

parse_asnswer_t parse_char_sequence(const std::string& regex, size_t offset)
{

    parse_asnswer_t current_ast_base = parse_char_sequence_element(regex, offset);
    if (!current_ast_base.m_success)
    {
        return parse_asnswer_t(offset);
    }
    else
    {
        offset = current_ast_base.m_offset;
    }

    parse_asnswer_t current_element;
    int count = 0;
    while (true)
    {
        current_element = parse_char_sequence_element(regex, offset);
        if (current_element.m_success)
        {
            current_ast_base.m_ast = std::move(std::make_unique<cat_op>(std::move(current_ast_base.m_ast), std::move(current_element.m_ast)));
            offset = current_element.m_offset;
        }
        else
        {
            current_ast_base.m_offset = offset;
            return current_ast_base;
        }
    }
}

parse_asnswer_t parse_char_class(const std::string& regex, size_t offset)
{

    if(offset < regex.size() && regex[offset] == '[')
    {
        ++offset;
    }
    else
    {
        return parse_asnswer_t(offset);
    }

    parse_asnswer_t current_ast_base = std::move(parse_char_sequence(regex, offset));
    if (!current_ast_base.m_success)
    {
        return current_ast_base;
    }
    offset = current_ast_base.m_offset;

    if(offset < regex.size() && regex[offset] == ']')
    {
        current_ast_base.m_offset = offset + 1;
        return current_ast_base;
    }
    else
    {
        return parse_asnswer_t(offset);
    }
}

parse_asnswer_t parse_char(const std::string& regex, size_t offset)
{

    if (offset < regex.size())
    {
        return parse_asnswer_t(true, std::make_unique<char_check>(regex[offset]), offset+1);
    }
    else
    {
        return parse_asnswer_t(offset);
    }
}

parse_asnswer_t parse_group(const std::string& regex, size_t offset)
{

    if(offset < regex.size() && regex[offset] == '(')
    {
        ++offset;
    }
    else
    {
        return parse_asnswer_t(offset);
    }

    parse_asnswer_t current_ast_base = std::move(parse_or(regex, offset));
    if (!current_ast_base.m_success)
    {
        return current_ast_base;
    }
    offset = current_ast_base.m_offset;

    if(offset < regex.size() && regex[offset] == ')')
    {
        current_ast_base.m_offset = offset + 1;
        return current_ast_base;
    }
    else
    {
        return parse_asnswer_t(offset);
    }
}

parse_asnswer_t parse_single_element(const std::string& regex, size_t offset)
{

    parse_asnswer_t current_ast_base;
    if (offset < regex.size())
    {
        switch (regex[offset])
        {
            case '(':
                current_ast_base = parse_group(regex, offset);
                break;

            case ')':
                current_ast_base = parse_asnswer_t(offset);
                break;

            case '[':
                current_ast_base = parse_char_class(regex, offset);
                current_ast_base.m_ast = std::move(std::make_unique<char_class_check>(std::move(current_ast_base.m_ast)));
                break;

            case ']':
                current_ast_base = parse_asnswer_t(offset);
                break;

            case '|':
                current_ast_base = parse_asnswer_t(offset);
                break;

            case '\\':
                ++offset;
                current_ast_base = parse_char(regex, offset);
                break;

            default:
                current_ast_base = parse_char(regex, offset);
                break;
        }
    }
    else
    {
        current_ast_base = parse_asnswer_t(offset);
    }
    return current_ast_base;
}

parse_asnswer_t parse_zero_or_more(const std::string& regex, size_t offset)
{

    parse_asnswer_t current_ast_base = std::move(parse_single_element(regex, offset));
    if(!current_ast_base.m_success)
    {
        return current_ast_base;
    }
    offset = current_ast_base.m_offset;

    if(offset < regex.size() && regex[offset] == '*')
    {
        current_ast_base.m_ast = std::move(std::make_unique<star_op>(std::move(current_ast_base.m_ast)));
        ++offset;
    }

    current_ast_base.m_offset = offset;
    return current_ast_base;
}

parse_asnswer_t parse_sequence(const std::string& regex, size_t offset)
{

    parse_asnswer_t current_ast_base = std::move(parse_zero_or_more(regex, offset));
    if(!current_ast_base.m_success)
    {
        return current_ast_base;
    }
    offset = current_ast_base.m_offset;

    parse_asnswer_t current_element;
    while(true)
    {
        current_element = std::move(parse_zero_or_more(regex, offset));

        // combine answers
        if(current_element.m_success)
        {
            current_ast_base.m_ast = std::make_unique<cat_op>(std::move(current_ast_base.m_ast), std::move(current_element.m_ast));
            offset = current_element.m_offset;
        }
        else
        {
            break;
        }
    }
    current_ast_base.m_offset = offset;
    return current_ast_base;
}

parse_asnswer_t parse_or(const std::string& regex, size_t offset)
{

    parse_asnswer_t current_ast_base = std::move(parse_sequence(regex, offset));
    if(!current_ast_base.m_success)
    {
        return current_ast_base;
    }
    else
    {
        offset = current_ast_base.m_offset;
    }
    while(offset < regex.size() && regex[offset] == '|')
    {
        ++offset;
        parse_asnswer_t current_element = std::move(parse_sequence(regex, offset));
        if(!current_element.m_success)
        {
            return current_element;
        }
        offset = current_element.m_offset;
        // combine answers
        current_ast_base.m_ast = std::make_unique<or_op>(std::move(current_ast_base.m_ast), std::move(current_element.m_ast));
    }
    current_ast_base.m_offset = offset;
    return current_ast_base;
}

parse_asnswer_t parse_regex(const std::string& regex)
{
    return parse_or(regex, 0);
}

}
}


