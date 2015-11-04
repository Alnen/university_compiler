#include <iostream>
#include "grammar.h"
#include "ControlTable.h"
#include "SyntaxAnalyzer.h"

#include <string>
#include <fstream>

template<class T>
void print_set(const Grammar::Set<T>& p_set)
{
	for (size_t i = 0; i < p_set.size(); ++i) {
		std::cout << p_set[i] << std::endl;
	}
}

int main()
{
	/*
	Grammar::Set<int> t_set = {1, 2, 3, 4, 5};
	Grammar::Set<int> t_set2 = { 3, 8, 9, 10, 11 };
	Grammar::Set<int> t_set3 = { 1, 2, 3, 4, 5 };
	Grammar::Set<int> t_set4 = { 8, 2, 3, 4, 5, 1, 9, 0 };
	Grammar::Set<int> t_set5 = { 3, 5 ,4 };
	*/
	//print_set<int>(t_set);
	//print_set<std::remove_reference<Grammar::Set<int>>::type>(t_set);
	//print_set<int>(t_set.union_set(t_set2));
	//print_set<int>(t_set.difference(t_set5));
	//print_set<int>(t_set.intersection(t_set5));
	//print_set<int>(t_set2.symmetric_difference(t_set5));
	//std::cout << (t_set >= t_set5) << std::endl;
	Grammar::TokenSet terminal_set = { {"+", 10}, {"*", 11}, {"i", 12}, {"(", 13}, {")", 14} };
	Grammar::RuleList rule_list = { { {"E", 100}, {{"T", 101}, { "E1", 102}} },
									{ {"E1", 102}, {{"+", 10}, {"T", 101}, {"E1", 102}} },
									{ {"E1", 102}, {{"epsilon", Grammar::EPSILON}} },
									{ {"T", 101}, {{"P", 103}, {"T1", 104}} },
									{ {"T1", 104}, {{"*", 11}, {"P", 103}, {"T1", 104}} },
									{ { "T1", 104 }, {{"epsilon", Grammar::EPSILON}} },
									{ { "P", 103 }, {{"i", 12}} },
									{ { "P", 103 }, {{"(", 13}, {"E", 100}, {")", 14}} }
								  };
	Grammar::Grammar grammar;
	//Grammar::Grammar grammar(terminal_set, rule_list);
	//grammar.start_symbol = Grammar::TokenType("E", 100);
	//grammar.readTerminals("reserved_words.txt");
	//grammar.readRules("rules.txt");
	//grammar.writeTerminals("out_rw.txt");
	//grammar.writeRules("out_rules.txt");
	std::cout << grammar.isLL1() << std::endl;
	/*
	Grammar::TokenSet f_set = grammar.first(Grammar::TokenType(Grammar::VarSection));
	for (size_t i = 0; i < f_set.size(); ++i) {
		std::cout << f_set[i].value << std::endl;
	}*/
	/*
	Grammar::ControlTable control_table(grammar);
	std::ofstream outfile("control_table.txt");
	for (size_t i = 0; i < control_table.column_names.size(); ++i) {
		outfile << " " << control_table.column_names[i];
	}
	outfile << std::endl;
	Grammar::Rule curr_rule;
	for (size_t i = 0; i < control_table.row_names.size(); ++i) {
		outfile << control_table.row_names[i] << " >> ";
		for (size_t j = 0; j < control_table.column_names.size(); ++j) {
			curr_rule = control_table.values[i*control_table.column_names.size() + j];
			if (curr_rule.left.value >= 0) {
				outfile << curr_rule.left.value << " ->";
				for (size_t k = 0; k < curr_rule.right.size(); ++k) {
					outfile << " " << curr_rule.right[k].value;
				}
			}
			else outfile << " None";
			outfile << " | ";
		}
		outfile << std::endl;
	}
	*/
	Grammar::SyntaxAnalyzer syntax_analyzer(grammar);
	int res = syntax_analyzer.readNextToken({ Grammar::RWPR });
	//int res = syntax_analyzer.readTokens({{ Grammar::RWPR }, { Grammar::ID }, { Grammar::SRSM }});
	std::cout << "SyntaxAnalyser result: " << res << std::endl;
	return 0;
}