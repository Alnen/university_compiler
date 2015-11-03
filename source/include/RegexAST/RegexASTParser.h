#ifndef REGEXASTPARSER_H
#define REGEXASTPARSER_H

#include <memory>
#include <string>

#include "RegexASTNode.h"

namespace Lexer {
namespace RegexAST {

class RegexToASTParser
{
public:
    struct parse_asnswer_t
    {
        bool m_success;
        std::unique_ptr<BasicNode> m_ast;

        parse_asnswer_t():
            m_success(false),
            m_ast()
        {
        }

        parse_asnswer_t(bool success, std::unique_ptr<BasicNode>&& ast):
            m_success(success),
            m_ast(std::move(ast))
        {
        }

        parse_asnswer_t(parse_asnswer_t&& rhs):
            m_success(rhs.m_success),
            m_ast(std::move(rhs.m_ast))
        {
        }

        void operator=(parse_asnswer_t&& rhs)
        {
            m_success = rhs.m_success;
            m_ast = std::move(rhs.m_ast);
        }

        parse_asnswer_t(const parse_asnswer_t&) = delete;
        parse_asnswer_t& operator=(const parse_asnswer_t&) = delete;
    };

    parse_asnswer_t parse(const std::string& regex);

protected:
    bool            parse_char_sequence_element(const std::string& regex, CharClassCheckLeaf& node);
    bool            parse_char_sequence(const std::string& regex, CharClassCheckLeaf& node);
    parse_asnswer_t parse_char_class(const std::string& regex);
    parse_asnswer_t parse_char(const std::string& regex);
    parse_asnswer_t parse_group(const std::string& regex);
    parse_asnswer_t parse_single_element(const std::string& regex);
    parse_asnswer_t parse_zero_or_more(const std::string& regex);
    parse_asnswer_t parse_sequence(const std::string& regex);
    parse_asnswer_t parse_or(const std::string& regex);

    BasicLeaf::LeafId m_idGenerator = 0;
    size_t m_offset = 0;
};

}
}

#endif // REGEXASTPARSER_H
