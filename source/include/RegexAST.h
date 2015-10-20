#ifndef REGEXAST_H
#define REGEXAST_H

#include <iosfwd>
#include <iomanip>
#include <memory>
#include <string>
#include <list>

namespace Lexer {
namespace RegexAST {

enum class RegexASTType
{
    CAT = 0,
    OR,
    STAR,
    SINGLE_CHAR,
    CHAR_CLASS,
    CHAR_RANGE
};

struct BasicNode
{
    const RegexASTType m_type;
    BasicNode(RegexASTType type): m_type(type) {}
    virtual void print(std::ostream& out, size_t offset) = 0;
    virtual ~BasicNode() {}
};

std::ostream& operator<<(std::ostream& out, BasicNode& elem);

struct StarNode : public BasicNode
{
    std::unique_ptr<BasicNode> m_elem;

    StarNode(std::unique_ptr<BasicNode>&& elem):
        BasicNode(RegexASTType::STAR),
        m_elem(std::move(elem))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "StarNode"  << std::endl;
        m_elem.get()->print(out, offset+4);
    }
};

struct OrNode : public BasicNode
{
    std::unique_ptr<BasicNode> m_rhsElem;
    std::unique_ptr<BasicNode> m_lhsElem;

    OrNode(std::unique_ptr<BasicNode>&& lhs, std::unique_ptr<BasicNode>&& rhs):
        BasicNode(RegexASTType::OR),
        m_lhsElem(std::move(lhs)),
        m_rhsElem(std::move(rhs))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "OrNode" << std::endl;
        m_lhsElem.get()->print(out, offset+4);
        m_rhsElem.get()->print(out, offset+4);
    }
};

struct CatNode : public BasicNode
{
    std::unique_ptr<BasicNode> m_rhsElem;
    std::unique_ptr<BasicNode> m_lhsElem;
    CatNode(std::unique_ptr<BasicNode>&& lhs, std::unique_ptr<BasicNode>&& rhs):
        BasicNode(RegexASTType::CAT),
        m_lhsElem(std::move(lhs)),
        m_rhsElem(std::move(rhs))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "CatNode" << std::endl;
        m_lhsElem.get()->print(out, offset+4);
        m_rhsElem.get()->print(out, offset+4);
    }
};

struct CharCheckNode : public BasicNode
{
    char m_elem;
    CharCheckNode(char elem):
        BasicNode(RegexASTType::SINGLE_CHAR),
        m_elem(elem)
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "CharCheckNode : " << m_elem << std::endl;
    }
};

struct CharClassCheckNode : public BasicNode
{
    std::unique_ptr<BasicNode> m_elem;

    CharClassCheckNode(std::unique_ptr<BasicNode>&& elem):
        BasicNode(RegexASTType::CHAR_CLASS),
        m_elem(std::move(elem))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "CharClassCheckNode"  << std::endl;
        m_elem.get()->print(out, offset+4);
    }
};

struct CharRangeCheckNode : public BasicNode
{
    char m_from;
    char m_to;
    CharRangeCheckNode(char from, char to):
        BasicNode(RegexASTType::CHAR_RANGE),
        m_from(from),
        m_to(to)
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "CharClassCheckNode : " << m_from << '-' << m_to << std::endl;
    }
};

struct parse_asnswer_t
{
    bool m_success;
    std::unique_ptr<BasicNode> m_ast;
    size_t m_offset;

    parse_asnswer_t(size_t offset = 0):
        m_success(false),
        m_ast(),
        m_offset(offset)
    {
    }

    parse_asnswer_t(bool success, std::unique_ptr<BasicNode>&& ast, size_t offset):
        m_success(success),
        m_ast(std::move(ast)),
        m_offset(offset)
    {
    }

    parse_asnswer_t(parse_asnswer_t&& rhs):
        m_success(rhs.m_success),
        m_ast(std::move(rhs.m_ast)),
        m_offset(rhs.m_offset)
    {
    }

    void operator=(parse_asnswer_t&& rhs)
    {
        m_success = rhs.m_success;
        m_ast = std::move(rhs.m_ast);
        m_offset = rhs.m_offset;
    }

    parse_asnswer_t(const parse_asnswer_t&) = delete;
    parse_asnswer_t& operator=(const parse_asnswer_t&) = delete;
};

parse_asnswer_t parse_char_sequence_element(const std::string& regex, size_t offset);
parse_asnswer_t parse_char_sequence(const std::string& regex, size_t offset);
parse_asnswer_t parse_char_class(const std::string& regex, size_t offset);
parse_asnswer_t parse_char(const std::string& regex, size_t offset);
parse_asnswer_t parse_group(const std::string& regex, size_t offset);
parse_asnswer_t parse_single_element(const std::string& regex, size_t offset);
parse_asnswer_t parse_zero_or_more(const std::string& regex, size_t offset);
parse_asnswer_t parse_sequence(const std::string& regex, size_t offset);
parse_asnswer_t parse_or(const std::string& regex, size_t offset);

}
}

#endif // REGEXAST_H
