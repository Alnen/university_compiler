#include "RegexAST/RegexASTAnnotationEvaluator.h"
#include "InputChecker/InputHandler.h"
#include "Utility.h"
#include "StdFix.h"

namespace Lexer {

std::ostream& operator << (std::ostream& out, IInputChecker* check)
{
    check->print(out);
    return out;
}

namespace RegexAST {

void RegexASTAnnotationEvaluator::evaluate_impl(BasicNode* ast)
{
    using Utility::construct_set_union;

    switch (ast->m_type)
    {
        case RegexASTType::CAT:
        {
            CatNode* node = static_cast<CatNode*>(ast);
            this->evaluate_impl(node->m_lhsElem.get());
            this->evaluate_impl(node->m_rhsElem.get());

            node->m_nullable = node->m_lhsElem->m_nullable && node->m_rhsElem->m_nullable;
            if (node->m_lhsElem->m_nullable)
            {
                node->m_firstpos = std::move(construct_set_union(node->m_lhsElem->m_firstpos, node->m_rhsElem->m_firstpos));
            }
            else
            {
                node->m_firstpos = node->m_lhsElem->m_firstpos;
            }

            if (node->m_rhsElem->m_nullable)
            {
                node->m_lastpos = std::move(construct_set_union(node->m_lhsElem->m_lastpos, node->m_rhsElem->m_lastpos));
            }
            else
            {
                node->m_lastpos = node->m_rhsElem->m_lastpos;
            }

            for (BasicLeaf::LeafId leaf : node->m_lhsElem->m_lastpos)
            {
                size_t leaf_table_offset = leaf - m_firstLeafId;
                std::copy_if(
                            node->m_rhsElem->m_firstpos.begin(),
                            node->m_rhsElem->m_firstpos.end(),
                            std::back_inserter(m_followpos_table[leaf_table_offset]),
                            [this, leaf_table_offset](BasicLeaf::LeafId value)
                            {
                                return std::find(
                                            m_followpos_table[leaf_table_offset].begin(),
                                            m_followpos_table[leaf_table_offset].end(),
                                            value
                                        ) == m_followpos_table[leaf_table_offset].end();
                            }
                );
            }
            break;
        }

        case RegexASTType::OR:
        {
            OrNode* node = static_cast<OrNode*>(ast);
            this->evaluate_impl(node->m_lhsElem.get());
            this->evaluate_impl(node->m_rhsElem.get());
            node->m_nullable = node->m_lhsElem->m_nullable || node->m_rhsElem->m_nullable;
            node->m_firstpos = std::move(construct_set_union(node->m_lhsElem->m_firstpos, node->m_rhsElem->m_firstpos));
            node->m_lastpos = std::move(construct_set_union(node->m_lhsElem->m_lastpos, node->m_rhsElem->m_lastpos));
            break;
        }

        case RegexASTType::STAR:
        {
            StarNode* node = static_cast<StarNode*>(ast);
            this->evaluate_impl(node->m_elem.get());
            node->m_nullable = true;
            node->m_firstpos = node->m_elem->m_firstpos;
            node->m_lastpos = node->m_elem->m_lastpos;
            for (BasicLeaf::LeafId leaf : node->m_elem->m_lastpos)
            {
                size_t leaf_table_offset = leaf - m_firstLeafId;
                std::copy_if(
                            node->m_elem->m_firstpos.begin(),
                            node->m_elem->m_firstpos.end(),
                            std::back_inserter(m_followpos_table[leaf_table_offset]),
                            [this, leaf_table_offset](BasicLeaf::LeafId value)
                            {
                                return std::find(
                                            m_followpos_table[leaf_table_offset].begin(),
                                            m_followpos_table[leaf_table_offset].end(),
                                            value
                                        ) == m_followpos_table[leaf_table_offset].end();
                            }
                );
            }
            break;
        }

        case RegexASTType::CHAR_CLASS:
        {
            CharClassCheckLeaf* node = static_cast<CharClassCheckLeaf*>(ast);
            node->m_firstpos.push_back(node->m_id);
            node->m_lastpos.push_back(node->m_id);
            node->m_nullable = false;
            ++m_numberOfLeaves;
            m_followpos_table.resize(m_numberOfLeaves);
            std::shared_ptr<CharClassChecker> checker = std::make_shared<CharClassChecker>(node->m_charChecks, node->m_charRangeChecks);
            m_inputToIdMapping[checker].push_back(node->m_id);
            break;
        }

        case RegexASTType::SINGLE_CHAR:
        {
            CharCheckLeaf* node = static_cast<CharCheckLeaf*>(ast);
            node->m_firstpos.push_back(node->m_id);
            node->m_lastpos.push_back(node->m_id);
            node->m_nullable = false;
            ++m_numberOfLeaves;
            m_followpos_table.resize(m_numberOfLeaves);
            std::shared_ptr<CharChecker> checker = std::make_shared<CharChecker>(node->m_elem);
            m_inputToIdMapping[checker].push_back(node->m_id);
            break;
        }
    }
}

}
}

