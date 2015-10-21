#ifndef REGEXAST_H
#define REGEXAST_H

#include <iosfwd>
#include <iomanip>
#include <list>
#include <memory>
#include <string>
#include <vector>


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

struct BasicLeaf : public BasicNode
{
    using LeafId = int;

    LeafId m_id;

    BasicLeaf(RegexASTType type, LeafId id): BasicNode(type), m_id(id) {}
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

struct CharCheckNode : public BasicLeaf
{
    char m_elem;

    CharCheckNode(LeafId id, char elem):
        BasicLeaf(RegexASTType::SINGLE_CHAR, id),
        m_elem(elem)
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "CharCheckNode : " << m_elem << std::endl;
    }
};

struct CharClassCheckNode : public BasicLeaf
{
    using CharRange = std::pair<char, char>;
    std::vector<char> m_charChecks;
    std::vector<CharRange> m_charRangeChecks;

    CharClassCheckNode(LeafId id):
        BasicLeaf(RegexASTType::CHAR_CLASS, id)
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "CharClassCheckNode"  << std::endl;
    }

    void addChar(char sybmol)
    {
        m_charChecks.push_back(sybmol);
    }

    void addCharRange(char from, char to)
    {
        m_charRangeChecks.emplace_back(from, to);
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

parse_asnswer_t parse_char_sequence_element(const std::string& regex, size_t offset, CharClassCheckNode& node);
parse_asnswer_t parse_char_sequence(const std::string& regex, size_t offset, CharClassCheckNode& node);
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
