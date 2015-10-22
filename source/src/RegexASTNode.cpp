#include "RegexASTNode.h"

namespace Lexer {
namespace RegexAST {

std::ostream& operator<<(std::ostream& out, BasicNode& elem)
{
    elem.print(out, 0);
    return out;
}

}
}


