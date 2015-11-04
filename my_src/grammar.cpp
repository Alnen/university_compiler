#include "grammar.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iterator>

Grammar::Grammar::Grammar() : start_symbol({Program})
{
	for (size_t i = TERMINAL_START_INDEX + 1;
	i < TERMINAL_FINISH_INDEX; ++i)
	{
		terminals.add(TokenType(i));
	}
	for (size_t i = NONTERMINAL_START_INDEX + 1;
		i < NONTERMINAL_FINISH_INDEX; ++i)
	{
		nonterminals.add(TokenType(i));
	}
	rules = {
		{{Program},{{RWPR},{ID},{SRSM},{Program1}}},
		{{Program1},{{ProcedureFunctions},{CompoundStatement},{SRSP}}},
		{{Program1},{{DescriptionSection1},{DescriptionSection},{ProcedureFunctions},{CompoundStatement},{SRSP}}},
		{{DescriptionSection},{{EPSILON}}},
		{{DescriptionSection},{{DescriptionSection1},{DescriptionSection}}},
		{{DescriptionSection1},{{LabelSection}}},
		{{DescriptionSection1},{{ConstSection}}},
		{{DescriptionSection1},{{TypeSection}}},
		{{DescriptionSection1},{{VarSection}}},
		{{DescriptionSection1},{{OperatorOverloadSection}}},
		{{ProcedureFunctions},{{Function},{ProcedureFunctions}}},
		{{ProcedureFunctions},{{Procedure},{ProcedureFunctions}}},
		{{ProcedureFunctions},{{EPSILON}}},
		{{LabelSection},{{RWLB},{Label},{LabelSection1},{SRSM}}},
		{{LabelSection1},{{EPSILON}}},
		{{LabelSection1},{{SRCA},{Label},{LabelSection1}}},
		{{Label},{{ID}}},
		{{ConstSection},{{RWCN},{ConstDefinition},{SRSM},{ConstSection1}}},
		{{ConstSection1},{{EPSILON}}},
		{{ConstSection1},{{ConstDefinition},{SRSM},{ConstSection1}}},
		{{ConstDefinition},{{ID},{OPEQ},{Expression}}},
		{{TypeSection},{{RWTP},{TypeDefinition},{SRSM},{TypeSection1}}},
		{{TypeSection1},{{EPSILON}}},
		{{TypeSection1},{{TypeDefinition},{SRSM},{TypeSection1}}},
		{{TypeDefinition},{{ID},{OPEQ},{Type}}},
		{{Type},{{ConcreteType}}},
		{{Type},{{Massive}}},
		{{Type},{{EnumType}}},
		{{Type},{{ID}}},
		{{ConcreteType},{{RWINT}}},
		{{ConcreteType},{{RWRAT}}},
		{{Massive},{{RWAR},{SRLB},{IndexType},{Massive1},{SRRB},{RWOF},{Type}}},
		{{Massive1},{{EPSILON}}},
		{{Massive1},{{SRCA},{IndexType},{Massive1}}},
		{{IndexType},{{EnumType}}},
		{{IndexType},{{ID}}},
		{{EnumType},{{SRLP},{IdList},{SRRP}}},
		{{ConstExpr},{{UnaryAdditiveOperator},{ConstExpr1}}},
		{{ConstExpr},{{ConstExpr1}}},
		{{ConstExpr1},{{ID}}},
		{{ConstExpr1},{{CI}}},
		{{ConstExpr1},{{CR}}},
		{{ConstExpr1},{{CX}}},
		{{VarSection},{{RWV},{VarSection1},{VarSection2}}},
		{{VarSection1},{{IdList},{SRCN},{Type},{SRSM}}},
		{{VarSection2},{{VarSection1},{VarSection2}}},
		{{VarSection2},{{EPSILON}}},
		{{IdList},{{ID},{IdList1}}},
		{{IdList1},{{SRCA},{ID},{IdList1}}},
		{{IdList1},{{EPSILON}}},
		{{Expression},{{LogicOr}}},
		{{LogicOr},{{LogicAnd},{LogicOr1}}},
		{{LogicOr1},{{RWLO},{LogicAnd},{LogicOr1}}},
		{{LogicOr1},{{EPSILON}}},
		{{LogicAnd},{{Relation},{LogicAnd1}}},
		{{LogicAnd1},{{RWLA},{Relation},{LogicAnd1}}},
		{{LogicAnd1},{{EPSILON}}},
		{{Relation},{{Expression1},{Relation1}}},
		{{Relation1},{{BinaryRelationOperator},{Expression1},{Relation1}}},
		{{Relation1},{{EPSILON}}},
		{{Expression1},{{Summand},{Expression2}}},
		{{Expression2},{{BinaryAdditiveOperator},{Summand},{Expression2}}},
		{{Expression2},{{EPSILON}}},
		{{Summand},{{Factor},{Factor1},{Summand1}}},
		{{Summand1},{{BinaryMulOperator},{Factor},{Factor1},{Summand1}}},
		{{Summand1},{{EPSILON}}},
		{{Factor1},{{RWCM},{Factor},{Factor1}}},
		{{Factor1},{{EPSILON}}},
		{{Factor},{{UnaryOperator},{Var1}}},
		{{Factor},{{Var1}}},
		{{Factor},{{SRLP},{LogicOr},{SRRP}}},
		{{Var1},{{RightHandVar}}},
		{{Var1},{{CI}}},
		{{Var1},{{CR}}},
		{{Var1},{{CX}}},
		{{Var},{{ID},{ExprList}}},
		{{ExprList},{{EPSILON}}},
		{{ExprList},{{SRLB},{Expression},{ExprList1},{SRRB}}},
		{{ExprList1},{{EPSILON}}},
		{{ExprList1},{{SRCA},{Expression},{ExprList1}}},
		{{LeftHandVar},{{Var},{LeftHandVar1}}},
		{{LeftHandVar1},{{LeftHandPostfixRationalOperator}}},
		{{LeftHandVar1},{{EPSILON}}},
		{{RightHandVar},{{Var},{RightHandVar1}}},
		{{RightHandVar1},{{RightHandPostfixRationalOperator}}},
		{{RightHandVar1},{{EPSILON}}},
		{{BinaryAdditiveOperator},{{OPPLUS}}},
		{{BinaryAdditiveOperator},{{OPMINUS}}},
		{{BinaryMulOperator},{{OPMUL}}},
		{{BinaryMulOperator},{{OPDIV}}},
		{{UnaryOperator},{{BinaryAdditiveOperator}}},
		{{UnaryOperator},{{UnaryOperator1}}},
		{{UnaryOperator1},{{RWINTOP}}},
		{{UnaryOperator1},{{RWIR}}},
		{{UnaryOperator1},{{RWSM}}},
		{{UnaryOperator1},{{RWFR}}},
		{{UnaryOperator1},{{RWLN}}},
		{{UnaryOperator1},{{RWIM}}},
		{{UnaryOperator1},{{RWRE}}},
		{{UnaryAdditiveOperator},{{BinaryAdditiveOperator}}},
		{{BinaryRelationOperator},{{OPGT}}},
		{{BinaryRelationOperator},{{OPLT}}},
		{{BinaryRelationOperator},{{OPGE}}},
		{{BinaryRelationOperator},{{OPLE}}},
		{{BinaryRelationOperator},{{OPEQ}}},
		{{BinaryRelationOperator},{{OPNE}}},
		{{PostfixRationalOperator},{{SRSP},{PostfixRationalOperator1}}},
		{{PostfixRationalOperator1},{{RWDN}}},
		{{PostfixRationalOperator1},{{RWN}}},
		{{LeftHandPostfixRationalOperator},{{PostfixRationalOperator}}},
		{{RightHandPostfixRationalOperator},{{PostfixRationalOperator}}},
		{{CompoundStatement},{{RWB},{Statement},{CompoundStatement1}}},
		{{CompoundStatement1},{{SRSM},{CompoundStatement2}}},
		{{CompoundStatement1},{{RWEND}}},
		{{CompoundStatement2},{{RWEND}}},
		{{CompoundStatement2},{{SRSM},{CompoundStatement2}}},
		{{CompoundStatement2},{{Statement2},{CompoundStatement1}}},
		{{Statement},{{EPSILON}}},
		{{Statement},{{Statement2}}},
		{{Statement2},{{ConditionalOperator}}},
		{{Statement2},{{CompoundStatement}}},
		{{Statement2},{{InputOperator}}},
		{{Statement2},{{OutputOperator}}},
		{{Statement2},{{UnconditionalJumpOperator}}},
		{{Statement2},{{LoopWithParameterOperator}}},
		{{Statement2},{{ID},{Statement1}}},
		{{Statement1},{{SRCN},{Statement}}},
		{{Statement1},{{ExprList},{LeftHandVar1},{OPAS},{Expression}}},
		{{LoopWithParameterOperator},{{RWFOR},{ID},{OPAS},{Expression},{LoopDirection},{Expression},{RWDO},{Statement}}},
		{{LoopDirection},{{RWDT}}},
		{{LoopDirection},{{RWTO}}},
		{{AssignmentOperator},{{LeftHandVar},{OPAS},{Expression}}},
		{{UnconditionalJumpOperator},{{RWGT},{Label}}},
		{{ConditionalOperator},{{RWIF},{Expression},{RWTH},{ConditionalOperator1}}},
		{{ConditionalOperator1},{{RWEL},{Statement2}}},
		{{ConditionalOperator1},{{Statement2},{ConditionalOperator2}}},
		{{ConditionalOperator2},{{RWEL},{Statement2}}},
		{{ConditionalOperator2},{{EPSILON}}},
		{{InputOperator},{{RWRD},{SRLP},{LeftHandVar},{InputOperator1},{SRRP}}},
		{{InputOperator1},{{SRCA},{LeftHandVar}}},
		{{OutputOperator},{{RWWR},{SRLP},{Expression},{OutputOperator1},{SRRP}}},
		{{OutputOperator1},{{SRCA},{Expression}}},
		{{OperatorOverloadSection},{{RWOP},{OperatorOverloadSection1}}},
		{{OperatorOverloadSection1},{{BinaryAdditiveOperator},{SRLP},{ID},{OperatorOverloadSection2}}},
		{{OperatorOverloadSection1},{{OverloadBinaryOperation1},{SRLP},{ID},{BinaryOperatorOverload1},{ID},{SRCN},{Type},{SRSM},{LocalDefinition1},{CompoundStatement},{SRSM}}},
		{{OperatorOverloadSection1},{{UnaryOperator1},{SRLP},{ID},{SRCN},{Type},{SRRP},{ID},{SRCN},{Type},{SRSM},{OperatorOverloadSection1},{CompoundStatement},{SRSM}}},
		{{OperatorOverloadSection2},{{SRCA},{ID},{SRCN},{Type},{SRRP},{ID},{SRCN},{Type},{SRSM},{LocalDefinition1},{CompoundStatement},{SRSM}}},
		{{OperatorOverloadSection2},{{SRCN},{Type},{OperatorOverloadSection3}}},
		{{OperatorOverloadSection3},{{SRSM},{ID},{SRCN},{Type},{SRRP},{ID},{SRCN},{Type},{SRSM},{LocalDefinition1},{CompoundStatement},{SRSM}}},
		{{OperatorOverloadSection3},{{SRRP},{ID},{SRCN},{Type},{SRSM},{OperatorOverloadSection1},{CompoundStatement},{SRSM}}},
		{{LocalDefinition1},{{EPSILON}}},
		{{LocalDefinition1},{{LocalDefinition},{OperatorOverloadSection1}}},
		{{LocalDefinition},{{TypeSection}}},
		{{LocalDefinition},{{VarSection}}},
		{{LocalDefinition},{{ConstSection}}},
		{{LocalDefinition},{{LabelSection}}},
		{{UnaryOperatorOverload},{{UnaryOperator},{SRLP},{ID},{SRCN},{Type},{SRRP}}},
		{{BinaryOperatorOverload},{{OverloadBinaryOperation},{SRLP},{ID},{BinaryOperatorOverload1}}},
		{{BinaryOperatorOverload1},{{SRCA},{ID},{SRCN},{Type},{SRRP}}},
		{{BinaryOperatorOverload1},{{SRCN},{Type},{SRSM},{ID},{SRCN},{Type},{SRRP}}},
		{{OverloadBinaryOperation},{{BinaryAdditiveOperator}}},
		{{OverloadBinaryOperation},{{OverloadBinaryOperation1}}},
		{{OverloadBinaryOperation1},{{BinaryMulOperator}}},
		{{OverloadBinaryOperation1},{{RWLO}}},
		{{OverloadBinaryOperation1},{{RWLA}}},
		{{OverloadBinaryOperation1},{{OPAS}}},
		{{OverloadBinaryOperation1},{{RWCM}}},
		{{Function},{{RWFUN},{ID},{SRLP},{FunctionArgs},{SRRP},{SRCN},{Type},{SRSM},{LocalDefinition1},{CompoundStatement},{SRSM}}},
		{{FunctionArgs},{{OneTypeArgs},{FunctionArgs1}}},
		{{FunctionArgs1},{{SRSM},{FunctionArgs}}},
		{{FunctionArgs1},{{EPSILON}}},
		{{OneTypeArgs},{{ID},{OneTypeArgs1}}},
		{{OneTypeArgs1},{{SRCA},{ID},{IdList1},{SRCN},{Type}}},
		{{OneTypeArgs1},{{SRCN},{Type}}},
		{{Procedure},{{RWPRC},{ID},{SRLP},{FunctionArgs},{ResultArgs},{SRRP},{SRCN},{Type},{SRSM},{LocalDefinition1},{CompoundStatement},{SRSM}}},
		{{ResultArgs},{{EPSILON}}},
		{{ResultArgs},{{SRSM},{RWV},{FunctionArgs}}}
	};
}

Grammar::Grammar::Grammar(const TokenSet& set_terminals, const RuleList& set_rules)
{
	terminals = set_terminals;
	for (size_t i = 0; i < set_rules.size(); ++i) {
		rules.push_back(set_rules[i]);
		int term_id = getTermId(set_rules[i].left.name, terminals, nonterminals, 2);
		if (term_id > 99) nonterminals.add(set_rules[i].left);
		for (size_t j = 0; j < set_rules[i].right.size(); ++j) {
			term_id = getTermId(set_rules[i].right[j].name, terminals, nonterminals, 2);
			if (term_id > 99) nonterminals.add(set_rules[i].right[j]);
		}
	}
}

Grammar::TokenSet Grammar::Grammar::readSetFromFile(const char* file_name, size_t start_index)
{
	std::ifstream infile(file_name);
	TokenSet r_set;
	std::string line = "";
	for (size_t i = start_index; std::getline(infile, line); ++i) {
		if (line.size() > 1) {
			line.pop_back();
			r_set.add(TokenType(line, i));
		}
	}
	infile.close();
	return r_set;
}

Grammar::RuleList Grammar::Grammar::rulesUnion(const RuleList& rules1, const RuleList& rules2) const
{
	RuleList rule_union = rules1;
	for (size_t i = 0; i < rules2.size(); ++i) {
		for (size_t j = 0; j < rule_union.size(); ++j) {
			if (rule_union[j] != rules2[i]) {
				rule_union.push_back(rules2[i]);
			}
		}
	}
	return rule_union;
}

void Grammar::Grammar::readTerminals(const char* file_name, int flag)
{
	std::ifstream infile(file_name);
	if (!infile.is_open()) return;
	TokenSet term_set;
	std::string line = "";
	while (std::getline(infile, line)) {
		if (line.size() > 1) {
			if (line[line.size()-1] == '\n' || line[line.size() - 1] == '\0')
				line.pop_back();
			term_set.add(TokenType(line, getTermId(line, term_set, 1)));
		}
	}
	infile.close();
	if (flag == 0)
		terminals = term_set;
	else
		terminals.update(term_set);
}

void Grammar::Grammar::writeTerminals(const char* file_name, int flag) const
{
	std::ofstream out(file_name, (flag == 0) ? std::ios_base::out : std::ios_base::app);
	for (size_t i = 0; i < terminals.size(); ++i) {
		if ((terminals[i].value != EPSILON) && (terminals[i].value != FINAL_STATE_SYMBOL)) {
			out << terminals[i].name << "\n";
		}
	}
}

void Grammar::Grammar::readNonterminals(const char* file_name, int flag)
{
	std::ifstream infile(file_name);
	if (!infile.is_open()) return;
	TokenSet term_set;
	std::string line = "";
	while (std::getline(infile, line)) {
		if (line.size() > 1) {
			if (line[line.size() - 1] == '\n' || line[line.size() - 1] == '\0')
				line.pop_back();
			term_set.add(TokenType(line, getTermId(line, term_set, 2)));
		}
	}
	infile.close();
	if (flag == 0)
		nonterminals = term_set;
	else
		nonterminals.update(term_set);
}

void Grammar::Grammar::writeNonterminals(const char* file_name, int flag) const
{
	std::ofstream out(file_name, (flag == 0) ? std::ios_base::out : std::ios_base::app);
	for (size_t i = 0; i < nonterminals.size(); ++i) {
		out << nonterminals[i].name << "\n";
	}
}

void Grammar::Grammar::readRules(const char* file_name, int write_nonterminals, int flag)
{
	RuleList new_rules;
	TokenSet new_nonterminals;
	Rule new_rule;
	std::ifstream infile(file_name);
	if (!infile.is_open()) return;
	std::string line = "";
	int state = 0; // 0 - read left part, 1 - read ->, 2 - read right part
	int term_id = -1;
	while (std::getline(infile, line)) {
		if ((line.compare("\n") != 0) &&
			(line.compare("\0") != 0))
		{
			std::istringstream iss(line);
			std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>{} };
			for (size_t i = 0; i < tokens.size(); ++i) {
				if (i == (tokens.size() - 1)) {
					if (state < 2) return;
					state = 0;
					term_id = getTermId(tokens[i], terminals, new_nonterminals, 2);
					if (term_id > 99) {
						new_nonterminals.add(TokenType(tokens[i], term_id));
					}
					new_rule.right.push_back(TokenType(tokens[i], term_id));
					new_rules.push_back(new_rule);
					new_rule.right.clear();
				}
				else {
					if (state == 0) {
						term_id = getTermId(tokens[i], terminals, new_nonterminals, 2);
						if (term_id > 99) {
							new_nonterminals.add(TokenType(tokens[i], term_id));
						}
						new_rule.left = TokenType(tokens[i], term_id);
						state = 1;
					}
					else if (state == 1) {
						if (tokens[i].compare("->") != 0) return;
						state = 2;
					}
					else {
						term_id = getTermId(tokens[i], terminals, new_nonterminals, 2);
						if (term_id > 99) {
							new_nonterminals.add(TokenType(tokens[i], term_id));
						}
						new_rule.right.push_back(TokenType(tokens[i], term_id));
					}
				}
			}
		}
	}
	if (flag == 0) {
		if (write_nonterminals == 1) {
			nonterminals = new_nonterminals;
		}
		rules = new_rules;
	}
	else {
		if (write_nonterminals == 1) {
			nonterminals.update(new_nonterminals);
		}
		rules = rulesUnion(rules, new_rules);
	}
}

void Grammar::Grammar::writeRules(const char* file_name, int flag) const
{
	std::ofstream out(file_name, (flag == 0) ? std::ios_base::out : std::ios_base::app);
	for (size_t i = 0; i < rules.size(); ++i) {
		out << rules[i].left.name << " ->";
		for (size_t j = 0; j < rules[i].right.size(); ++j) {
			out << " " << rules[i].right[j].name;
		}
		out << "\n";
	}
}

bool Grammar::Grammar::isTerminal(TokenType symbol) const
{
	for (size_t i = 0; i < terminals.size(); ++i) {
		if (terminals[i] == symbol)
			return true;
	}
	return false;
}

bool Grammar::Grammar::isNonterminal(TokenType symbol) const
{
	for (size_t i = 0; i < nonterminals.size(); ++i) {
		if (nonterminals[i] == symbol)
			return true;
	}
	return false;
}

int Grammar::Grammar::getTermId(std::string term_name, const TokenSet& term_set) const
{
	for (size_t i = 0; i < term_set.size(); ++i) {
		if (term_name.compare(term_set[i].name) == 0) {
			return term_set[i].value;
		}
	}
	return -1;
}

int Grammar::Grammar::getTermId(std::string term_name, const TokenSet& term_set, int new_termimal) const
{
	std::transform(term_name.begin(), term_name.end(), term_name.begin(), tolower);
	if (term_name.compare("epsilon") == 0) {
		return EPSILON;
	}
	if (term_name.compare("$") == 0) {
		return FINAL_STATE_SYMBOL;
	}
	if (new_termimal == 1) {
		if (((term_name[0] == 'i') && (term_name[1] == 'd')) ||
			((term_name[0] == 'c') && ((term_name[1] == 'i') || (term_name[1] == 'r') || (term_name[1] == 'x')))
			)
		{
			term_name.resize(2);
		}
	}
	int term_id = getTermId(term_name, term_set);
	if (term_id >= 0) return term_id;
	if (new_termimal > 0) {
		if (term_set.size() == 0) {
			return (new_termimal == 1 ? 10 : 100);
		}
		else {
			return term_set[term_set.size() - 1].value + 1;
		}
	}
	return -1;
}

int Grammar::Grammar::getTermId(std::string term_name, const TokenSet& terminal_set, const TokenSet& nonterminal_set, int new_termimal) const
{
	std::string t_name = term_name;
	TokenSet t_set = terminal_set;
	TokenSet nt_set = nonterminal_set;
	for (size_t i = 0; i < t_set.size(); ++i) {
		std::transform(t_set[i].name.begin(), t_set[i].name.end(), t_set[i].name.begin(), toupper);
	}
	for (size_t i = 0; i < nt_set.size(); ++i) {
		std::transform(nt_set[i].name.begin(), nt_set[i].name.end(), nt_set[i].name.begin(), toupper);
	}
	std::transform(term_name.begin(), term_name.end(), term_name.begin(), toupper);
	if (term_name.compare("EPSILON") == 0) {
		return EPSILON;
	}
	if (term_name.compare("$") == 0) {
		return FINAL_STATE_SYMBOL;
	}
	int term_id = getTermId(term_name, nt_set);
	if (term_id >= 0) return term_id;
	if (((term_name[0] == 'I') && (term_name[1] == 'D')) ||
		((term_name[0] == 'C') && ((term_name[1] == 'I') || (term_name[1] == 'R') || (term_name[1] == 'X')))
		)
	{
		term_name.resize(2);
	}
	term_id = getTermId(term_name, t_set);
	if (term_id >= 0) return term_id;
	if (new_termimal == 1) {
		if (terminal_set.size() == 0) {
			return 10;
		}
		else {
			return terminal_set[terminal_set.size() - 1].value + 1;
		}
	}
	if (new_termimal == 2) {
		if (nonterminal_set.size() == 0) {
			return 100;
		}
		else {
			return nonterminal_set[nonterminal_set.size() - 1].value + 1;
		}
	}
	return -1;
}

int Grammar::Grammar::getTermId(std::string term_name, int new_termimal) const
{
	return getTermId(term_name, terminals, nonterminals, new_termimal);
}

Grammar::TokenSet Grammar::Grammar::first(TokenType A, TokenSet& checked_rules) const
{
	TokenSet first_set;
	if ((isTerminal(A)) || (A.value == EPSILON)) {
		first_set.add(A);
		return first_set;
	}
	checked_rules.add(A);
	for (size_t i = 0; i < rules.size(); ++i) {
		if (rules[i].left == A) {
			if (rules[i].right[0].value == EPSILON) {
				first_set.add(TokenType(EPSILON));
			}
			else {
				for (size_t j = 0; j < rules[i].right.size(); ++j) {
					if (!checked_rules.contain(rules[i].right[j])) {
						TokenSet f_set = first(rules[i].right[j], checked_rules);
						if (f_set.size() == 0) continue;
						if (f_set.contain(TokenType(EPSILON))) {
							for (size_t k = 0; k < f_set.size(); ++k) {
								if ((f_set[k].value != EPSILON) || (i == (rules[i].right.size() - 1))) {
									first_set.add(f_set[k]);
								}
							}
						}
						else {
							first_set.update(f_set);
							break;
						}
					}
				}
			}
		}
	}
	return first_set;
}

Grammar::TokenSet Grammar::Grammar::ruleFirst(std::vector<TokenType> A) const
{
	TokenSet first_set;
	TokenSet checked_rules;
	if (A.size() == 0)
		return first_set;
	if (A[0].value == EPSILON) {
		first_set.add(TokenType(EPSILON));
	}
	else {
		for (size_t i = 0; i < A.size(); ++i) {
			if (!checked_rules.contain(A[i])) {
				TokenSet f_set = first(A[i], checked_rules);
				if (f_set.size() == 0) continue;
				if (f_set.contain(TokenType(EPSILON))) {
					for (size_t k = 0; k < f_set.size(); ++k) {
						if ((f_set[k].value != EPSILON) || (i == (A.size() - 1))) {
							first_set.add(f_set[k]);
						}
					}
				}
				else {
					first_set.update(f_set);
					break;
				}
			}
		}
	}
	return first_set;
}

Grammar::TokenSet Grammar::Grammar::first(TokenType A) const
{
	TokenSet checked_rules;
	return first(A, checked_rules);
}

Grammar::TokenSet Grammar::Grammar::follow(TokenType A, TokenSet& checked_rules) const
{
	TokenSet follow_set;
	checked_rules.add(A);
	if (A == start_symbol) {
		follow_set.add(TokenType(FINAL_STATE_SYMBOL));
	}
	for (size_t i = 0; i < rules.size(); ++i) {
		for (size_t j = 0; j < rules[i].right.size(); ++j) {
			if (rules[i].right[j] == A) {
				if (j == (rules[i].right.size() - 1)) {
					if (!checked_rules.contain(rules[i].left)) {
						TokenSet f_set = follow(rules[i].left, checked_rules);
						if (f_set.size() > 0) follow_set.update(f_set);
					}
				}
				else {
					TokenSet f_set = first(rules[i].right[j + 1]);
					if (f_set.size() == 0) continue;
					for (size_t k = 0; k < f_set.size(); ++k) {
						if (f_set[k].value != EPSILON) {
							follow_set.add(f_set[k]);
						}
					}
					if (f_set.contain(TokenType(EPSILON)) && (!checked_rules.contain(rules[i].left))) {
						f_set = follow(rules[i].left, checked_rules);
						if (f_set.size() > 0) follow_set.update(f_set);
					}
				}
			}
		}
	}
	return follow_set;
}

Grammar::TokenSet Grammar::Grammar::follow(TokenType A) const
{
	TokenSet checked_rules;
	return follow(A, checked_rules);
}

bool Grammar::Grammar::isLL1() const
{
	for (size_t i = 0; i < nonterminals.size(); ++i) {
		std::vector<std::vector<TokenType>> t_rules;
		for (size_t j = 0; j < rules.size(); ++j) {
			if (nonterminals[i] == rules[j].left) {
				t_rules.push_back(rules[j].right);
			}
		}
		for (size_t j = 0; j < t_rules.size(); ++j) {
			for (size_t k = j + 1; k < t_rules.size(); ++k) {
				TokenSet f_set = ruleFirst(t_rules[j]);
				TokenSet f_set2 = ruleFirst(t_rules[k]);
				TokenSet fset_intersection = f_set.intersection(f_set2);
				if (fset_intersection.size() > 0) {
					return false;
				}
				if (f_set.contain(TokenType(EPSILON))) {
					TokenSet f_set3 = follow(nonterminals[i]);
					TokenSet fset_intersection2 = f_set3.intersection(f_set2);
					if (fset_intersection2.size() > 0) {
						return false;
					}
				}
			}
		}
	}
	return true;
}