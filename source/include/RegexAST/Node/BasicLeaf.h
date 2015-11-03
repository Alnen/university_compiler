#ifndef REGEXASTBASICLEAF_H
#define REGEXASTBASICLEAF_H

#include "BasicNode.h"

namespace Lexer {
namespace RegexAST {

struct BasicLeaf : public BasicNode
{
    using LeafId = BasicNode::IndexType;

    BasicLeaf(RegexASTType type, LeafId id): BasicNode(type), m_id(id) {}

    LeafId m_id;
};

}
}

#endif // REGEXASTBASICLEAF_H
