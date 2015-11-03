#ifndef REGEXASTANNOTATIONEVALUATOR_H
#define REGEXASTANNOTATIONEVALUATOR_H

#include <algorithm>

#include <boost/container/flat_map.hpp>

#include "InputChecker/IInputHandler.h"
#include "RegexASTNode.h"
#include "Utility.h"

namespace Lexer {
namespace RegexAST {

class RegexASTAnnotationEvaluator
{
public:
    using answer_t = std::pair<
                            boost::container::flat_map<std::shared_ptr<IInputChecker>, std::vector<BasicLeaf::LeafId>, PointerLess>,
                            std::vector<std::vector<BasicLeaf::LeafId>>
                              >;

    answer_t evaluate(BasicNode* ast, BasicLeaf::LeafId first_leaf_id)
    {
        m_numberOfLeaves = 0;
        m_followpos_table.clear();
        m_inputToIdMapping.clear();
        m_firstLeafId = first_leaf_id;

        evaluate_impl(ast);

        return std::make_pair(std::move(m_inputToIdMapping), std::move(m_followpos_table));
    }

protected:
    void evaluate_impl(BasicNode* ast);

    boost::container::flat_map<std::shared_ptr<IInputChecker>, std::vector<BasicLeaf::LeafId>, PointerLess> m_inputToIdMapping;
    std::vector<std::vector<BasicLeaf::LeafId>> m_followpos_table;
    BasicLeaf::LeafId m_firstLeafId;
    size_t m_numberOfLeaves = 0;
};

}
}

#endif // REGEXASTANNOTATIONEVALUATOR_H
