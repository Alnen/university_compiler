#ifndef REGEXASTSTARNODE_H
#define REGEXASTSTARNODE_H

#include <ostream>
#include <memory>

#include "BasicNode.h"

namespace Lexer {
namespace RegexAST {

struct StarNode : public BasicNode
{
    StarNode(std::unique_ptr<BasicNode>&& elem):
        BasicNode(RegexASTType::STAR),
        m_elem(std::move(elem))
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "StarNode"  << std::endl;
        m_elem.get()->print(out, offset+4);
    }

    std::unique_ptr<BasicNode> m_elem;
};

}
}

#endif // REGEXASTSTARNODE_H
