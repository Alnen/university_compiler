#ifndef REGEXASTORNODE_H
#define REGEXASTORNODE_H

#include <memory>

#include "BasicNode.h"

namespace Lexer {
namespace RegexAST {

struct OrNode : public BasicNode
{
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

    std::unique_ptr<BasicNode> m_rhsElem;
    std::unique_ptr<BasicNode> m_lhsElem;
};

}
}

#endif // REGEXASTORNODE_H
