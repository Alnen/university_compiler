#ifndef REGEXASTCATNODE_H
#define REGEXASTCATNODE_H

#include <memory>

#include "BasicNode.h"

namespace Lexer {
namespace RegexAST {

struct CatNode : public BasicNode
{
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

    std::unique_ptr<BasicNode> m_rhsElem;
    std::unique_ptr<BasicNode> m_lhsElem;
};

}
}

#endif // REGEXASTCATNODE_H
