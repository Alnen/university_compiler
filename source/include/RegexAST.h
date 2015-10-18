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

struct element
{
    const RegexASTType m_type;
    element(RegexASTType type): m_type(type) {}
    virtual void print(std::ostream& out, size_t offset) = 0;
    virtual ~element() {}
};

std::ostream& operator<<(std::ostream& out, element& elem);

struct star_op : public element
{
    std::unique_ptr<element> m_elem;

    star_op(std::unique_ptr<element>&& elem):
        element(RegexASTType::STAR),
        m_elem(std::move(elem))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "star_op"  << std::endl;
        m_elem.get()->print(out, offset+4);
    }
};

struct or_op : public element
{
    std::unique_ptr<element> m_rhsElem;
    std::unique_ptr<element> m_lhsElem;

    or_op(std::unique_ptr<element>&& lhs, std::unique_ptr<element>&& rhs):
        element(RegexASTType::OR),
        m_lhsElem(std::move(lhs)),
        m_rhsElem(std::move(rhs))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "or_op" << std::endl;
        m_lhsElem.get()->print(out, offset+4);
        m_rhsElem.get()->print(out, offset+4);
    }
};

struct cat_op : public element
{
    std::unique_ptr<element> m_rhsElem;
    std::unique_ptr<element> m_lhsElem;
    cat_op(std::unique_ptr<element>&& lhs, std::unique_ptr<element>&& rhs):
        element(RegexASTType::CAT),
        m_lhsElem(std::move(lhs)),
        m_rhsElem(std::move(rhs))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "cat_op" << std::endl;
        m_lhsElem.get()->print(out, offset+4);
        m_rhsElem.get()->print(out, offset+4);
    }
};

struct char_check : public element
{
    char m_elem;
    char_check(char elem):
        element(RegexASTType::SINGLE_CHAR),
        m_elem(elem)
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "char_check : " << m_elem << std::endl;
    }
};

struct char_class_check : public element
{
    std::unique_ptr<element> m_elem;

    char_class_check(std::unique_ptr<element>&& elem):
        element(RegexASTType::CHAR_CLASS),
        m_elem(std::move(elem))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "char_class_check"  << std::endl;
        m_elem.get()->print(out, offset+4);
    }
};

struct char_range_check : public element
{
    char m_from;
    char m_to;
    char_range_check(char from, char to):
        element(RegexASTType::CHAR_RANGE),
        m_from(from),
        m_to(to)
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "char_class_check : " << m_from << '-' << m_to << std::endl;
    }
};

struct parse_asnswer_t
{
    bool m_success;
    std::unique_ptr<element> m_ast;
    size_t m_offset;

    parse_asnswer_t(size_t offset = 0):
        m_success(false),
        m_ast(),
        m_offset(offset)
    {
    }

    parse_asnswer_t(bool success, std::unique_ptr<element>&& ast, size_t offset):
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
