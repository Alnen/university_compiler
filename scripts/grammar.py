__author__ = 'Alexandr'

import re

def read_set_from_file(file_name):
    r_set = set()
    with open(file_name, 'r') as f:
        for line in f:
            term = line.rstrip('\n')
            r_set.add(term)
    return r_set

def rules_union(rules1, rules2):
    rule_union = rules1
    for r2 in rules2:
        for r1 in rule_union:
            if r1 != r2:
                rule_union.append(r2)
    rule_union = sorted(rule_union)
    return rule_union

class Grammar:
    def __init__(self):
        self.terminals = set()
        self.nonterminals = set()
        self.rules = []
        self.start_symbol = 'S'

    def read_terminals(self, file_name, flag = 0):
        term_set = read_set_from_file(file_name)
        if flag == 0:
            self.terminals = term_set
        else:
            self.terminals = self.terminals.union(term_set)
        return

    def write_terminals(self, file_name, flag = 0):
        write_mode = 'w' if flag == 0 else 'a'
        t = sorted(list(self.terminals))
        with open(file_name, write_mode) as f:
            i = 10
            for term in t:
                if term != 'epsillon':
                    f.write(term+', // '+str(i)+'\n')
                    i += 1
        return

    def read_nonterminals(self, file_name, flag = 0):
        nonterm_set = read_set_from_file(file_name)
        if flag == 0:
            self.nonterminals = nonterm_set
        else:
            self.nonterminals = self.terminals.union(nonterm_set)
        return

    def write_nonterminals(self, file_name, flag = 0):
        write_mode = 'w' if flag == 0 else 'a'
        nt = sorted(list(self.nonterminals))
        with open(file_name, write_mode) as f:
            i = 100
            for term in nt:
                f.write(term+', // '+str(i)+'\n')
                i += 1
        return

    def read_rules(self, file_name, write_nonterminals = 1, flag = 0):
        rules = []
        nonterminals = set()
        with open(file_name, 'r') as f:
            for line in f:
                if line != '\n' and line != '\0':
                    terms = line.rstrip('\n').split(' ')
                    rules.append((terms[0], terms[2:]))
                    nonterminals.add(terms[0])
        if flag == 0:
            if write_nonterminals == 1:
                self.nonterminals = nonterminals
            self.rules = rules
        else:
            if write_nonterminals == 1:
                self.nonterminals = self.nonterminals.union(nonterminals)
            self.rules = rules_union(self.rules, rules)
        return

    def read_rules2(self, file_name, write_nonterminals = 1):
        rules = []
        nonterminals = set()
        header_str = ""
        state = 0
        with open(file_name, 'r') as f:
            line_number = 0
            for line in f:
                line_number += 1
                m = re.search('ProcedureDeclaration', line)
                if m:
                    t = 0
                if state == 0:
                    # read header
                    header_str += line
                    m = re.search('\{', line)
                    if m:
                        state = 1
                elif state == 1:
                    # read rules
                    m1 = re.search('^[\t\s]*//', line)
                    m2 = re.search('^[\t\s]*\n+[\t\s]*$', line)
                    if (not m1) and (not m2):
                        m = re.search('};', line)
                        if not m:
                            rule_parts = line.strip(' \t\n').split('{')
                            rule_head = rule_parts[1].split(',')[0]
                            rule_tail_str = rule_parts[2].split('}')[0]
                            rule_tail = []
                            m = re.search(',', rule_tail_str)
                            if m:
                                rule_tail = rule_tail_str.split(', ')
                            else:
                                rule_tail.append(rule_tail_str)
                            #print('rule_head: ', rule_head)
                            #print('rule_tail: ', rule_tail)
                            rules.append((rule_head, rule_tail))
                            '''
                            v = rules.get(rule_head, None)
                            if v is None:
                                rules[rule_head] = rule_tail
                            else:
                                rules[rule_head].append(rule_tail)
                            '''
                            #rules.append((rule_head, rule_tail))
                            nonterminals.add(rule_head)
        if write_nonterminals == 1:
            self.nonterminals = nonterminals
        self.rules = rules
        return

    def write_rules(self, file_name, flag = 0):
        write_mode = 'w' if flag == 0 else 'a'
        with open(file_name, write_mode) as f:
            f.write('{\n')
            i = 0
            for rule in self.rules:
                f.write('{{'+rule[0]+'},{')
                j = 0
                for term in rule[1]:
                    if term == 'epsilon':
                        term = 'EPSILON'
                    if (j == 0):
                        f.write('{'+term+'}')
                    else:
                        f.write(',{'+term+'}')
                    j += 1
                if i == (len(self.rules) - 1):
                    f.write('}}\n')
                else:
                    f.write('}},\n')
                i += 1
            f.write('}\n')
        return

    def is_terminal(self, symbol):
        if symbol in self.terminals:
            return 1
        return 0

    def is_nonterminal(self, symbol):
        if self.is_terminal(symbol) == 0:
            return 1
        return 0