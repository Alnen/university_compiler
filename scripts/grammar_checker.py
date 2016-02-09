import re

def read_nonterminals(file_name):
    nonterminal_set = set()
    actions_set = set()
    special_symbols_set = set()
    state = 0
    header_str = ""
    nterms_start_index = -1
    nterms_count = 0
    actions_start_index = -1
    actions_count = 0
    word_max_len = 0
    line_number = 0
    with open(file_name, 'r') as f:
        for line in f:
            line_number += 1
            if state == 0:
                # read header
                header_str += line
                m = re.search('\{', line)
                if m:
                    state = 1
            elif state == 1:
                # read nonterms start index
                m = re.search('[\t\s]*NONTERMINAL_START_INDEX', line)
                if m:
                    word_max_len = len('NONTERMINAL_START_INDEX')+1
                    m = re.search('[\t\s]*\d+[\t\s]*,', line)
                    if m:
                        nterms_start_index = int(m.group(0).split(',')[0].strip())
                        state = 2
                    else:
                        print("NONTERMINALS_ERROR (line ", line_number, "): Can\'t find nonterminals start index")
            elif state == 2:
                # read nonterminals
                m = re.search('\w+[\t\s]*,', line)
                if m:
                    nonterm_str = m.group(0)
                    nonterm = nonterm_str.split(',')[0].strip()
                    word_len = len(nonterm)
                    if word_len > word_max_len:
                        word_max_len = word_len
                    if nonterm == "NONTERMINAL_FINISH_INDEX":
                        state = 3
                    else:
                        nterms_count += 1
                        nonterminal_set.add(nonterm)
                else:
                    print("NONTERMINALS_ERROR (line ", line_number, "): Can\'t find nonterminal")
            elif state == 3:
                # read action start index
                m = re.search('[\t\s]*ACTION_START_INDEX', line)
                if m:
                    if m:
                        actions_start_index = nterms_start_index + nterms_count + 1
                        state = 4
                    else:
                        print("NONTERMINALS_ERROR (line ", line_number, "): Can\'t find actions start index")
            elif state == 4:
                # read actions
                m = re.search('\w+[\t\s]*,', line)
                if m:
                    action_str = m.group(0)
                    action = action_str.split(',')[0].strip()
                    word_len = len(action)
                    if word_len > word_max_len:
                        word_max_len = word_len
                    if action == "ACTION_FINISH_INDEX":
                        state = 5
                    else:
                        actions_count += 1
                        actions_set.add(action)
                else:
                    print("NONTERMINALS_ERROR (line ", line_number, "): Can\'t find action")
            else:
                # read special symbols
                m = re.search('\w+[\t\s]*', line)
                if m:
                    special_symbol = m.group(0).strip()
                    word_len = len(special_symbol)
                    if word_len > word_max_len:
                        word_max_len = word_len
                    special_symbols_set.add(special_symbol)
                else:
                    break
    return (nonterminal_set, actions_set, special_symbols_set, nterms_start_index, header_str, word_max_len)

def write_nonterminals(file_name, sets):
    nonterminal_list = sorted(list(sets[0]))
    actions_list = sorted(list(sets[1]))
    special_symbols_list = sorted(list(sets[2]))
    with open(file_name, 'w') as f:
        i = sets[3]
        f.write(sets[4])
        word = 'NONTERMINAL_START_INDEX'
        f.write(word.rjust(len(word)+4).ljust(sets[5]+4) + '  = ' + str(i) + ',\n')
        i += 1
        for nt in nonterminal_list:
            f.write((nt+',').rjust(len(nt)+5).ljust(sets[5]+5) + ' // ' + str(i) + '\n')
            i += 1
        word = 'NONTERMINAL_FINISH_INDEX'
        f.write((word+',').rjust(len(word)+5).ljust(sets[5]+5) + ' // ' + str(i) + '\n')
        i += 1
        word = 'ACTION_START_INDEX'
        f.write((word+',').rjust(len(word)+5).ljust(sets[5]+5) + ' // ' + str(i) + '\n')
        i += 1
        for action in actions_list:
            f.write((action+',').rjust(len(action)+5).ljust(sets[5]+5) + ' // ' + str(i) + '\n')
            i += 1
        word = 'ACTION_FINISH_INDEX'
        f.write((word+',').rjust(len(word)+5).ljust(sets[5]+5) + ' // ' + str(i) + '\n')
        i += 1
        j = 0
        while j < len(special_symbols_list):
            word = special_symbols_list[j]
            if j != len(special_symbols_list)-1:
                word += ','
            else:
                word += ' '
            f.write(word.rjust(len(word)+4).ljust(sets[5]+5) + ' // ' + str(i) + '\n')
            j += 1
            i += 1
        f.write('};')
    return

def read_terminals(file_name):
    term_set = set()
    state = 0
    header_str = ""
    terms_start_index = -1
    terms_count = 0
    word_max_len = 0
    line_number = 0
    with open(file_name, 'r') as f:
        for line in f:
            line_number += 1
            if state == 0:
                # read header
                header_str += line
                m = re.search('\{', line)
                if m:
                    state = 1
            elif state == 1:
                # read terms start index
                m = re.search('[\t\s]*TERMINAL_START_INDEX', line)
                if m:
                    word_max_len = len('TERMINAL_START_INDEX')+1
                    m = re.search('[\t\s]*\d[\t\s]*,', line)
                    if m:
                        terms_start_index = int(m.group(0).split(',')[0].strip())
                        state = 2
                    else:
                        print("TERMINALS_ERROR (line ", line_number, "): Can\'t find terminals start index")
            elif state == 2:
                # read terminals
                m = re.search('TERMINAL_FINISH_INDEX', line)
                if m:
                    break
                m = re.search('\w+[\t\s]*,', line)
                if m:
                    term_str = m.group(0)
                    term = term_str.split(',')[0].strip()
                    word_len = len(term)
                    if word_len > word_max_len:
                        word_max_len = word_len
                    terms_count += 1
                    term_set.add(term)
                else:
                    print("TERMINALS_ERROR (line ", line_number, "): Can\'t find terminal")
    return (term_set, terms_start_index, header_str, word_max_len)

def write_terminals(file_name, sets):
    terminal_list = sorted(list(sets[0]))
    with open(file_name, 'w') as f:
        i = sets[1]
        f.write(sets[2])
        word = 'TERMINAL_START_INDEX'
        f.write(word.rjust(len(word)+4).ljust(sets[3]+4) + '  = ' + str(i) + ',\n')
        i += 1
        for term in terminal_list:
            f.write((term+',').rjust(len(term)+5).ljust(sets[3]+5) + ' // ' + str(i) + '\n')
            i += 1
        word = 'TERMINAL_FINISH_INDEX'
        f.write(word.rjust(len(word)+4).ljust(sets[3]+5) + ' // ' + str(i) + '\n')
        f.write('};')
    return

def remove_actions(grammar, action_set):
    new_rules = []
    for rule in grammar.rules:
        if rule[0] in action_set:
            continue
        right_part = []
        for term in rule[1]:
            if term not in action_set:
                right_part.append(term)
        new_rules.append((rule[0], right_part))
    grammar.rules = new_rules
    return

def write_set(file_name, in_set):
    el_list = sorted(list(in_set))
    with open(file_name, 'w') as f:
        for el in el_list:
            f.write(el+'\n')
    return

def write_nonterminal_map(file_name, sets):
    nonterminal_list = sorted(list(sets[0]))
    actions_list = sorted(list(sets[1]))
    special_symbols_list = sorted(list(sets[2]))
    header_str = "std::vector<boost::container::flat_map<NonterminalSymbols, std::string>::value_type> nonterminalTypePairsMapping =\n{\n"
    with open(file_name, 'w') as f:
        i = sets[3]
        f.write(header_str)
        prefix = '{ NonterminalSymbols::'
        max_len1 = len(prefix) + sets[5]
        max_len2 = sets[5] + 3
        word = 'NONTERMINAL_START_INDEX'
        f.write((prefix+word+',').rjust(len(prefix+word)+5).ljust(max_len1+5))
        f.write((' \"'+word+'\"').ljust(max_len2))
        f.write(' }, // ' + str(i) + '\n')
        i += 1
        for nt in nonterminal_list:
            f.write((prefix+nt+',').rjust(len(prefix+nt)+5).ljust(max_len1+5))
            f.write((' \"'+nt+'\"').ljust(max_len2))
            f.write(' }, // ' + str(i) + '\n')
            i += 1
        word = 'NONTERMINAL_FINISH_INDEX'
        f.write((prefix+word+',').rjust(len(prefix+word)+5).ljust(max_len1+5))
        f.write((' \"'+word+'\"').ljust(max_len2))
        f.write(' }, // ' + str(i) + '\n')
        i += 1
        word = 'ACTION_START_INDEX'
        f.write((prefix+word+',').rjust(len(prefix+word)+5).ljust(max_len1+5))
        f.write((' \"'+word+'\"').ljust(max_len2))
        f.write(' }, // ' + str(i) + '\n')
        i += 1
        for action in actions_list:
            word = action
            f.write((prefix+word+',').rjust(len(prefix+word)+5).ljust(max_len1+5))
            f.write((' \"'+word+'\"').ljust(max_len2))
            f.write(' }, // ' + str(i) + '\n')
            i += 1
        word = 'ACTION_FINISH_INDEX'
        f.write((prefix+word+',').rjust(len(prefix+word)+5).ljust(max_len1+5))
        f.write((' \"'+word+'\"').ljust(max_len2))
        f.write(' }, // ' + str(i) + '\n')
        i += 1
        j = 0
        while j < len(special_symbols_list):
            word = special_symbols_list[j]
            f.write((prefix+word+',').rjust(len(prefix+word)+5).ljust(max_len1+5))
            f.write((' \"'+word+'\"').ljust(max_len2))
            if j != len(special_symbols_list)-1:
                f.write(' }, // ' + str(i) + '\n')
            else:
                f.write(' }  // ' + str(i) + '\n')
            j += 1
            i += 1
        f.write('};')
    return

def sort_lines(input_file, output_file):
    # read lines from input_file
    sorted_lines = []
    with open(input_file, 'r') as f:
        for line in f:
            sorted_lines.append(line.lstrip(' \t\n'))
    sorted_lines = sorted(sorted_lines)
    # write sorted lines to output_file
    with open(output_file, 'w') as f:
        for line in sorted_lines:
            f.write(line)
    return


if __name__ == "__main__":
    print("read nonterm")
    #nonterm_sets = read_nonterminals('nterms.txt')
    #write_nonterminals('outnt.txt', nonterm_sets)
    #term_sets = read_terminals('terms.txt')
    #write_terminals('outterm.txt', term_sets)
    print("end")
    #print(sorted(sets[0]))