__author__ = 'Alexandr'
import re
from grammar import Grammar
from grammar_checker import *

grammar = Grammar()
grammar.read_rules2('gr_rules.txt')
nterm_sets = read_nonterminals('nterms.txt')
remove_actions(grammar, nterm_sets[1])
write_set('ntd1.txt', grammar.nonterminals)

#unique_terms1 = grammar.nonterminals.difference(nterm_sets[0])
#unique_terms2 = nterm_sets[0].difference(grammar.nonterminals)

#nterm_sets[0] = grammar.nonterminals
nterm_sets2 = (grammar.nonterminals, nterm_sets[1], nterm_sets[2], nterm_sets[3], nterm_sets[4], nterm_sets[5])
write_nonterminals('outnt.txt', nterm_sets2)

write_nonterminal_map('nt_map.txt', nterm_sets2)

sort_lines('in_hana_action_map.txt', 'out_hana_action_map.txt')

#write_set('ntd1.txt', unique_terms1)
#write_set('ntd2.txt', unique_terms2)

#remove_actions(grammar, nterm_sets[1]):
#print('nterm_set: ', nterm_sets[0])
#grammar.write_rules("out_rules.txt")
'''
grammar.write_rules("out_rules.txt")
grammar.read_terminals('reserved_words.txt')
grammar.read_rules('rules.txt')
grammar.write_terminals('termanals.txt')
grammar.write_nonterminals('nontermanals.txt')
grammar.write_rules("out_rules.txt")
grammar.start_symbol = 'Program'
'''

grammar7 = Grammar()
grammar7.rules = [('E',['T', 'E1']), ('E1', ['+', 'T', 'E1']), ('E1', ['epsilon']),
                  ('T', ['P', 'T1']), ('T1', ['*', 'P', 'T1']), ('T1', ['epsilon']), ('P', ['i']), ('P', ['(', 'E', ')'])]
grammar7.terminals = {'+','*','i','(',')','epsilon'}
grammar7.start_symbol = 'E'

def one_step_first(A, grammar, checked_rules):
    first_set = set()
    if (grammar.is_terminal(A) == 1) or (A == 'epsilon'):
        first_set.add(A)
        return first_set
    checked_rules.add(A)
    for t_rule in grammar.rules:
        if t_rule[0] == A:
            if t_rule[1] == ['epsilon']:
                first_set.add('epsilon')
            else:
                i = 0
                for term in t_rule[1]:
                    if term not in checked_rules:
                        f_set = one_step_first(term, grammar, checked_rules)
                        if f_set:
                            if 'epsilon' in f_set:
                                for f_term in f_set:
                                    if (f_term != 'epsilon') or (i == (len(t_rule[1])-1)):
                                        first_set.add(f_term)
                            else:
                                first_set.update(f_set)
                                break
                    i += 1
    return first_set

def r_first(A, grammar):
    checked_rules = set()
    first_set = set()
    if A == ['epsilon']:
        first_set.add('epsilon')
    else:
        i = 0
        for term in A:
            if term not in checked_rules:
                f_set = one_step_first(term, grammar, checked_rules)
                if f_set:
                    if 'epsilon' in f_set:
                        for f_term in f_set:
                            if (f_term != 'epsilon') or (i == (len(A)-1)):
                                first_set.add(f_term)
                    else:
                        first_set.update(f_set)
                        break
            i += 1
    return first_set

def first(A, grammar):
    checked_rules = set()
    if type(A) is list:
        return r_first(A, grammar)
    return one_step_first(A, grammar, checked_rules)

#print("first: ", first('TypeSection1', grammar))
#print("r_first: ", r_first(['epsilon', 'SRSM', 'TypeSection1'], grammar))
#print("first: ", first('E', grammar7))
#print("r_first: ", r_first(['P','T1'], grammar7))

def one_step_follow(A, grammar, checked_rules):
    follow_set = set()
    checked_rules.add(A)
    if A == grammar.start_symbol:
        follow_set.add('$')
    for t_rule in grammar.rules:
        i = 0
        while i < len(t_rule[1]):
            if t_rule[1][i] == A:
                if i == len(t_rule[1]) - 1:
                    if t_rule[0] not in checked_rules:
                        #print("one_step_follow(", t_rule[0],")")
                        f_set = one_step_follow(t_rule[0], grammar, checked_rules)
                        #print("one_step_follow(", t_rule[0],"): ", f_set)
                        if f_set:
                            follow_set.update(f_set)
                else:
                    f_set = first(t_rule[1][i+1:], grammar)
                    #print("ff_first(", t_rule[1][i+1],"): ",f_set)
                    if f_set:
                        for f_term in f_set:
                            if f_term != 'epsilon':
                                follow_set.add(f_term)
                        if ('epsilon' in f_set) and (t_rule[0] not in checked_rules):
                            #print("one_step_follow(", t_rule[0],")")
                            f_set = one_step_follow(t_rule[0], grammar, checked_rules)
                            #print("one_step_follow(", t_rule[0],"): ", f_set)
                            if f_set:
                                follow_set.update(f_set)
            i += 1
    return follow_set

def follow(A, grammar):
    checked_rules = set()
    return one_step_follow(A, grammar, checked_rules)

#print("follow: ", follow('TypeSection1', grammar))
#print("follow: ", follow('T1',grammar7))

def is_LL1_grammar(grammar):
    # check fisrt condition: A->a, A->b => first(a) ^ first(b) = 0
    keys = set()
    for rule in grammar.rules:
        keys.add(rule[0])
    for term in keys:
        t_rules = []
        for rule in grammar.rules:
            if term == rule[0]:
                t_rules.append(rule[1])
        i = 0
        for rule in t_rules:
            j = i + 1
            while j < len(t_rules):
                f_set = first(rule, grammar)
                f_set2 = first(t_rules[j], grammar)
                fset_intersection = f_set.intersection(f_set2)
                if fset_intersection:
                    print("First condition failed: (term: ", term,"; rule1: ", rule, "; rule2: ", t_rules[j])
                    print("intersection: ", fset_intersection)
                    return 0
                if 'epsilon' in f_set:
                    #print("follow(",term,")")
                    f_set3 = follow(term, grammar)
                    fset_intersection2 = f_set3.intersection(f_set2)
                    print("term: ",term," intersection: ",fset_intersection2)
                    if fset_intersection2:
                        #print("follow(",term,"): ",f_set3)
                        #print("f_set2: ",f_set2)
                        print("Second condition failed: (term: ", term,"; rule1: ", rule, "; rule2: ", t_rules[j])
                        print("intersection: ", fset_intersection2)
                        return 0
                j += 1
            i += 1
    return 1

print("Grammar is LL(1): ", is_LL1_grammar(grammar))
#print("Grammar is LL(1): ", is_LL1_grammar(grammar7))

def get_term_body_and_number(term):
    term_number = re.search('\d*$', term).group(0)
    term_body = term
    if term_number:
        term_body = term[:-len(term_number)]
    else:
        term_number = -1
    return (term_body, term_number)

def term_max_number(term_body, grammar):
    g_terms = set()
    for rule in grammar.rules:
        g_terms.add(rule[0])
    max_number = -2 # -1 - find term_body without number, -2 - this term_body not found
    for g_term in g_terms:
        tn = re.search('\d*$', g_term).group(0)
        t_number = -1
        t_body = g_term
        if tn:
            t_number = int(tn)
            t_body = g_term[:-len(tn)]
        if (t_body == term_body) and (t_number > max_number):
            max_number = t_number
    return max_number

def get_new_term(term, grammar):
    (term_body, term_number) = get_term_body_and_number(term)
    max_number = term_max_number(term_body, grammar)
    if max_number < -1:
        return term
    if max_number == -1:
        return term_body + '1'
    else:
        max_number += 1
        return term_body + str(max_number)

#print("get_new_term: ", get_new_term('Program', grammar))

def term_number_range(term_body, grammar):
    g_terms = set()
    for rule in grammar.rules:
        g_terms.add(rule[0])
    number_range = [] # -1 - find term_body without number
    for g_term in g_terms:
        tn = re.search('\d*$', g_term).group(0)
        t_number = -1
        t_body = g_term
        if tn:
            t_number = int(tn)
            t_body = g_term[:-len(tn)]
        if (t_body == term_body) and (t_number not in number_range):
            number_range.append(t_number)
    return sorted(number_range)

grammar3 = Grammar()
grammar3.rules = [('A2', ['a', 'A2', 'A6']), ('A2', ['u', 'o', 'A6']), ('A6', ['A6']), ('A6', ['n']),('A6', ['j', 'A6']), ('A3', ['h'])]
grammar3.terminals = {'a','u','o','n','j','h','epsilon'}
grammar3.start_symbol = 'A2'

grammar4 = Grammar()
grammar4.rules = [('A', ['a', 'A', 'A0']), ('A', ['u', 'o', 'A0']), ('A0', ['A0']), ('A0', ['n']),('A0', ['j', 'A0']), ('A3', ['h'])]
grammar4.terminals = {'a','u','o','n','j','h','epsilon'}
grammar4.start_symbol = 'A'

def change_term(old_term, new_term, grammar):
    i = 0
    while i < len(grammar.rules):
        if grammar.rules[i][0] == old_term:
            grammar.rules[i] = (new_term, grammar.rules[i][1])
        j = 0
        while j < len(grammar.rules[i][1]):
            if grammar.rules[i][1][j] == old_term:
               grammar.rules[i][1][j] = new_term
            j += 1
        i += 1
    return

#change_term('A6', 'B', grammar3)

def increment_term_numbers(term_body, number_range, start_index, inc, grammar, flag):
    if inc < 0:
        i = start_index
        while i < len(number_range):
            old_term = term_body
            if (number_range[i] > 0) or ((flag == 0) and (number_range[i] == 0)):
                old_term = old_term + str(number_range[i])
            new_term = term_body
            new_number = number_range[i] + inc
            if (new_number > 0) or ((flag == 0) and (new_number == 0)):
                new_term = new_term + str(new_number)
            change_term(old_term, new_term, grammar)
            number_range[i] += inc
            i += 1
    else:
        i = len(number_range) - 1
        while(i >= start_index):
            old_term = term_body
            if (number_range[i] > 0) or ((flag == 0) and (number_range[i] == 0)):
                old_term = old_term + str(number_range[i])
            new_term = term_body
            new_number = number_range[i] + inc
            if (new_number > 0) or ((flag == 0) and (new_number == 0)):
                new_term = new_term + str(new_number)
            change_term(old_term, new_term, grammar)
            number_range[i] += inc
            i -= 1
    return

def remove_rules_with_null(g_term, grammar):
    (term_body, term_number) = get_term_body_and_number(g_term)
    number_range = term_number_range(term_body, grammar)
    if number_range:
        i = 0
        while (i < len(number_range)) and (i < 2):
            if number_range[i] == 0:
                break
            i += 1
        if (i < 2) and (number_range[i] == 0):
            old_term = term_body+'0'
            new_term = get_new_term(old_term, grammar)
            change_term(old_term, new_term, grammar)
    return

#print("before remove_rules_with_null: ", grammar4.rules)
#remove_rules_with_null('A', grammar4)
#print("after remove_rules_with_null: ", grammar4.rules)
#print("number_range: ", term_number_range('A', grammar3))

def optimize_term_rules(term, grammar):
    (term_body, term_number) = get_term_body_and_number(term)
    remove_rules_with_null(term, grammar)
    # remove escaped numbers
    number_range = term_number_range(term_body, grammar)
    if not number_range:
        return
    if number_range[0] < 0:
        number_range[0] = 0
    i = 1
    while i < len(number_range):
        delta = number_range[i] - number_range[i-1]
        if delta > 1:
            increment_term_numbers(term_body, number_range, i, 1 - delta, grammar, 1)
        i += 1
    # reduce numbers
    if number_range[0] > 0:
        increment_term_numbers(term_body, number_range, 0, -number_range[0], grammar, 1)
    return

#print("before optimize_term_rules: ", grammar3.rules)
#optimize_term_rules('A', grammar3)
#print("after optimize_term_rules: ", grammar3.rules)

grammar5 = Grammar()
grammar5.rules = [('A',['B', 'C']), ('B',['B']),('B',['C']), ('C',['a']), ('D', ['o'])]
#grammar5.rules = {'A': [['a', 'A', 'B'], ['u', 'o', 'B']], 'B': [['B'],['n'],['j', 'B']], 'A3': [['h']]}
grammar5.terminals = {'a','u','o','n','j','h'}
grammar5.start_symbol = 'A'

'''
A -> aBc
A -> ac
B -> mA
B -> Cb
C -> tA
'''

grammar9 = Grammar()
grammar9.rules = {'A':[['a', 'B', 'c'], ['a','c']], 'B': [['m', 'A'],['C', 'b']], 'C': [['t', 'A']], 'L' : [['A']]}
#grammar5.rules = {'A': [['a', 'A', 'B'], ['u', 'o', 'B']], 'B': [['B'],['n'],['j', 'B']], 'A3': [['h']]}
grammar9.terminals = {'a','b','c','m','t'}
grammar9.start_symbol = 'A'

'''
def find_generating_nonterms(grammar, generating_nonterms):
    while(1):
        new_generating_nonterms = generating_nonterms.copy()
        for nonterm in grammar.rules.keys():
            if (not new_generating_nonterms) or (nonterm not in new_generating_nonterms):
                nt_rules = grammar.rules.get(nonterm)
                for nt_rule in nt_rules:
                    next_rule = 0
                    for term in nt_rule:
                        if ((grammar.is_terminal(term) == 0) and
                            ((not new_generating_nonterms) or (term not in new_generating_nonterms))):
                            next_rule = 1
                            break
                    if next_rule == 0:
                        new_generating_nonterms.add(nonterm)
                        break
        #print('New_gen_set: ', new_generating_nonterms)
        if len(new_generating_nonterms) == len(generating_nonterms):
            return new_generating_nonterms
        else:
            generating_nonterms = new_generating_nonterms
'''

def find_generating_nonterms(grammar, generating_nonterms):
    while(1):
        new_generating_nonterms = generating_nonterms.copy()
        for nonterm in grammar.rules.keys():
            if (not new_generating_nonterms) or (nonterm not in new_generating_nonterms):
                nt_rules = grammar.rules.get(nonterm)
                for nt_rule in nt_rules:
                    next_rule = 0
                    for term in nt_rule:
                        if ((grammar.is_terminal(term) == 0) and
                            ((not new_generating_nonterms) or (term not in new_generating_nonterms))):
                            next_rule = 1
                            break
                    if next_rule == 0:
                        new_generating_nonterms.add(nonterm)
                        break
        #print('New_gen_set: ', new_generating_nonterms)
        if len(new_generating_nonterms) == len(generating_nonterms):
            return new_generating_nonterms
        else:
            generating_nonterms = new_generating_nonterms

def generating_nonterminals(grammar):
    generating_nonterms = set()
    return find_generating_nonterms(grammar, generating_nonterms)

def find_nongenerating_nonterminals(grammar):
    gen_nt = generating_nonterminals(grammar)
    nongen_nt = set()
    for nonterm in grammar.rules.keys():
        if nonterm not in gen_nt:
            nongen_nt.add(nonterm)
    return nongen_nt

#print('generating nonterm: ', generating_nonterminals(grammar9))
#print('nongenerating nonterm: ', find_nongenerating_nonterminals(grammar9))
print('nongenerating nonterm: ', find_nongenerating_nonterminals(grammar))

def find_reachable_terms(grammar, reachable_terms):
    while(1):
        new_reachable_terms = reachable_terms.copy()
        for term in reachable_terms:
            if (grammar.is_terminal(term) == 0):
                nt_rules = grammar.rules.get(term)
                for nt_rule in nt_rules:
                    for t in nt_rule:
                        new_reachable_terms.add(t)
        #print('New_reachable_set: ', new_reachable_nonterms)
        if len(new_reachable_terms) == len(reachable_terms):
            return new_reachable_terms
        else:
            reachable_terms = new_reachable_terms

def reachable_term_set(grammar):
    reachable_terms = set()
    reachable_terms.add(grammar.start_symbol)
    return find_reachable_terms(grammar, reachable_terms)

def find_unreachable_terms(grammar):
    reachable_terms = reachable_term_set(grammar)
    unreachable_terms = set()
    for term in grammar.terminals:
        if term not in reachable_terms:
            unreachable_terms.add(term)
    for nonterm in grammar.rules.keys():
        if nonterm not in reachable_terms:
            unreachable_terms.add(nonterm)
    return unreachable_terms

#print('reachable terms: ', reachable_term_set(grammar9))
#print('unreachable terms: ', find_unreachable_terms(grammar9))
print('unreachable terms: ', find_unreachable_terms(grammar))

