#ifndef REGEXASTCHARCLASSCHECKLEAF_H
#define REGEXASTCHARCLASSCHECKLEAF_H

#include <memory>

#include "BasicLeaf.h"

namespace Lexer {
namespace RegexAST {

struct CharClassCheckLeaf : public BasicLeaf
{
    using CharRange = std::pair<char, char>;
    std::vector<char> m_charChecks;
    std::vector<CharRange> m_charRangeChecks;

    CharClassCheckLeaf(LeafId id):
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

}
}

#endif // REGEXASTCHARCLASSCHECKLEAF_H
