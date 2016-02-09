__author__ = 'Alexandr'
from grammar import Grammar
from check_useless_symbols import *

class ControlTable:
    def __init__(self):
        self.row_names = []
        self.column_names = []
        self.values = []

    def add(self, row, col, value):
        i = 0
        while i < len(self.row_names):
            if self.row_names[i] == row:
                break
            i += 1
        j = 0
        while j < len(self.column_names):
            if self.column_names[j] == col:
                break
            j += 1
        self.values[i][j] = value
        return
    '''
    def print_ct(self):
        with open('control_table.txt', 'w') as f:
            for term in self.column_names:
                f.write(' '+term)
            f.write('\n')
            for term in self.values:
                #
                f.write(term+', // '+str(i)+'\n')
        return
    '''

    def create(self, grammar):
        print("grammar.terminals: ", grammar.terminals)
        self.row_names = sorted(list(grammar.nonterminals))
        self.column_names = sorted(list(grammar.terminals)) + ['$']
        i = 0
        while i < len(self.row_names):
            r_list = []
            j = 0
            while j < len(self.column_names):
                r_list.append(None)
                j += 1
            self.values.append(r_list)
            i += 1
        for rule in grammar.rules:
            for row in self.row_names:
                if row == rule[0]:
                    f_set = first(rule[1], grammar)
                    for f_term in f_set:
                        if f_term != 'epsilon':
                            self.add(row, f_term, rule[1])
                    if 'epsilon' in f_set:
                        f_set = follow(row, grammar)
                        for f_term in f_set:
                            self.add(row, f_term, rule[1])
                        if '$' in f_set:
                            self.add(row, '$', rule[1])
                    break
        return

grammar7 = Grammar()
grammar7.rules = [('E',['T', 'E1']), ('E1', ['+', 'T', 'E1']), ('E1', ['epsilon']),
                  ('T', ['P', 'T1']), ('T1', ['*', 'P', 'T1']), ('T1', ['epsilon']), ('P', ['i']), ('P', ['(', 'E', ')'])]
grammar7.terminals = {'+','*','i','(',')'}
grammar7.nonterminals = {'E', 'T', 'E1', 'P', 'T1'}
grammar7.start_symbol = 'E'

control_table = ControlTable()
control_table.create(grammar)

print("rows: ", control_table.row_names)
print("columns: ", control_table.column_names)
print("values: ")
print("  ",control_table.column_names)
i = 0
while i < len(control_table.row_names):
    print(control_table.values[i], control_table.row_names[i])
    i += 1

i = 0
while i < len(control_table.row_names):
    print(control_table.values[i], control_table.row_names[i])
    i += 1

lex_tokens = [
    'ID',
    'CJ',
    'CI',
    'RWLA',
    'RWAR',
    'RWB',
    'RWCM',
    'RWCN',
    'RWDO',
    'RWDT',
    'RWEL',
    'RWEND',
    'RWFOR',
    'RWFR',
    'RWGT',
    'RWIF',
    'RWINT',
    'RWIR',
    'RWLB',
    'RWLN',
    'RWOF',
    'RWOP',
    'RWLO',
    'RWPR',
    'RWRAT',
    'RWRD',
    'RWSM',
    'RWTH',
    'RWTO',
    'RWTP',
    'RWV',
    'RWWR',
    'RWCX',
    'RWFUN',
    'RWIM',
    'RWRE',
    'RWINTOP',
    'RWPRC',
    'RWDN',
    'RWN',
    'OPPLUS',
    'OPMINUS',
    'OPMUL',
    'OPDIV',
    'OPGT',
    'OPLT',
    'OPGE',
    'OPLE',
    'OPEQ',
    'OPNE',
    'OPAS',
    'SRLP',
    'SRRP',
    'SRLB',
    'SRRB',
    'SRSM',
    'SRCN',
    'SRSP',
    'SRCA',
    'SRLCB',
    'SRRCB',
    'ERROR',
    'ENDOFFILE'
]

def write_terminals():
    write_mode = 'w'
    t = lex_tokens
    with open("out_terms.txt", write_mode) as f:
        i = 1
        for term in t:
            if term != 'epsillon':
                f.write(term+', // '+str(i)+'\n')
                i += 1
    return

write_terminals()

yacc_tokens = [
    'rules.emplace_back(TokenType::OPPLUS,   std::string("+#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPMINUS,  std::string("-#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPMUL,    std::string("\\*#"),                            std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPDIV,    std::string("/#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPGT,     std::string(">#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPLT,     std::string("<#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPGE,     std::string(">=#"),                             std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPLE,     std::string("<=#"),                             std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPEQ,     std::string("=#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPNE,     std::string(":=#"),                             std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::OPAS,     std::string("<>#"),                             std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::SRLP,     std::string("\\(#"),                            std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::SRRP,     std::string("\\)#"),                            std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::SRLB,     std::string("\\[#"),                            std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::SRRB,     std::string("\\]#"),                            std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::SRSM,     std::string(";#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::SRCN,     std::string(":#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::SRSP,     std::string(".#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::SRCA,     std::string(",#"),                              std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::CR,       std::string("([1-9][0-9]*|0).0*[1-9][0-9]*#"),    std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::CI,       std::string("[1-9][0-9]*#"),                    std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::CX,       std::string("([1-9][0-9]*|([1-9][0-9]*|0).0*[1-9][0-9]*)j#"),                    std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::CX,       std::string("{ *([1-9][0-9]*|([1-9][0-9]*|0).0*[1-9][0-9]*) *, *([1-9][0-9]*|([1-9][0-9]*|0).0*[1-9][0-9]*) *}#"),                    std::move(std::make_unique<IDHandler>()));',
    'rules.emplace_back(TokenType::ID,       std::string("[a-zA-Z][a-zA-Z0-9]*#"),           std::move(std::make_unique<IDHandler>()));'
]

import re

def func_cmp():
    print('----------------------')
    for y_str in yacc_tokens:
        m = re.search('TokenType::\w+', y_str)
        str2 = m.group(0).split('TokenType::')[1]
        i = 0
        while i < len(lex_tokens):
            if (lex_tokens[i] == str2):
                break
            i += 1
        if (i == len(lex_tokens)):
            print("ERROR")
            return
    print("SUCCESS")
    print('----------------------')
    '''
    if len(lex_tokens) != len(yacc_tokens):
        print("SIZE NOT MATCH: ",len(lex_tokens)," ",len(yacc_tokens))
    i = 0
    not_found = []
    while i < len(yacc_tokens):
        j = 0
        while j < len(lex_tokens):
            if (lex_tokens[j] == yacc_tokens[i]):
                break
            j += 1
        if (j == len(lex_tokens)):
            not_found.append(yacc_tokens[i])
            #print("ERROR: ",lex_tokens[i])
        i += 1
    print("SUCCESS: ",not_found)
    '''
    return

#func_cmp()