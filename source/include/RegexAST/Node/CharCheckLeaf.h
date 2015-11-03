#ifndef REGEXASTCHARCHECKLEAF_H
#define REGEXASTCHARCHECKLEAF_H

#include <memory>

#include "BasicLeaf.h"

namespace Lexer {
namespace RegexAST {

struct CharCheckLeaf : public BasicLeaf
{
    CharCheckLeaf(LeafId id, char elem):
        BasicLeaf(RegexASTType::SINGLE_CHAR, id),
        m_elem(elem)
    {}

    void print(std::ostream& out, size_t offset)
    {
        out << std::string(offset, ' ') << "CharCheckNode : " << m_elem << std::endl;
    }

    char m_elem;
};

}
}

#endif // REGEXASTCHARCHECKLEAF_H
