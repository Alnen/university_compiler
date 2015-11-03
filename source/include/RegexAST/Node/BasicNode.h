#ifndef REGEXASTBASICNODE_H
#define REGEXASTBASICNODE_H

#include <ostream>
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

    const RegexASTType      m_type;
    std::vector<IndexType>  m_firstpos;
    std::vector<IndexType>  m_lastpos;
    bool                    m_nullable = false;
};

std::ostream& operator<<(std::ostream& out, BasicNode& elem);

}
}

#endif // REGEXASTBASICNODE_H
