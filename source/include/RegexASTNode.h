#ifndef REGEXASTNODE_H
#define REGEXASTNODE_H

#include <iosfwd>
#include <iomanip>
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
    CHAR_CLASS
};

struct BasicNode
{  
    using IndexType = int;

    BasicNode(RegexASTType type): m_type(type) {}
    virtual void print(std::ostream& out, size_t offset) = 0;
    virtual ~BasicNode() {}

    const RegexASTType m_type;
    std::vector<IndexType>  m_firstpos;
    std::vector<IndexType>  m_lastpos;
    bool                    m_nullable = false;
};

struct BasicLeaf : public BasicNode
{
    using LeafId = BasicNode::IndexType;

    LeafId m_id;

    BasicLeaf(RegexASTType type, LeafId id): BasicNode(type), m_id(id) {}
};

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

std::ostream& operator<<(std::ostream& out, BasicNode& elem);
}

}

#endif // REGEXASTNODE_H
