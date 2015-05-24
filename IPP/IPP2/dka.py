#!/usr/bin/python3.2

#DKA:xpiste04

# standardni knihovny:
import re
import sys
import codecs
from os import path, access, R_OK

# moje:
import argparser
import fsm as my

# zpracujeme parametry
params = argparser.Params()
args = params.parse()

# zjisteni odkud bude cist vstup
input_handler = sys.stdin

if args.input:
    if path.isfile(args.input) and access(args.input, R_OK):
        input_handler = codecs.open(args.input, 'r', 'utf-8')
    else:
        print("Either file missing or is not readable")
        exit(2)

# zjisteni kam se vypise vystup
output_handler = sys.stdout

if args.output:
    try:
        output_handler = codecs.open(args.output, 'w', 'utf-8')
    except IOError:
        print("Either file missing or is not writable")
        exit(3)


########################################################################################################################
########################################################################################################################

# nacteni vstupu do retezce
input_string = input_handler.read()
input_handler.close()

if args.case_insensitive:
    input_string = input_string.lower()

# zbaveni se komentaru:
input_string = re.sub(r"(?<!')#[^\n]*[^$]", "", input_string)

# zakladni kontrola (zda obsahuje petici) + extrahovani jednotlivych casti KA
match = re.match(r"\s*\(\s*{(.*)}\s*,\s*{(.*)}\s*,\s*{(.*)}\s*,\s*(\w+)\s*,\s*{(.*)}\s*\)\s*", input_string, re.DOTALL)

if not match or match.group().__len__() != input_string.__len__():
    print("ERR: Vstupni automat nesplnuje popstana lexikalni a syntakticka pravidla!", file=sys.stderr)
    exit(40)

states = match.group(1)
input_symbols = match.group(2)
rules = match.group(3)
start_state = match.group(4)
final_states = match.group(5)


#
# Kontrola jednotlivych casti KA


def validator(regex, string):
    """ Fce aplikuje na retezec (string) zadany reg. vyraz (regex) a pokud vysledek nepokryje cely
        retezec ukonci skript s chybou. """
    match_obj = re.match(regex, string)

    if not match_obj or match_obj.group().__len__() != string.__len__():
        print("ERR: Vstupni automat nesplnuje popstana lexikalni a syntakticka pravidla!", file=sys.stderr)
        exit(40)

    return

# STAVY:

if len(states) != 0:
    # odstraneni prebytechnych mezer
    states = re.sub(r"(?:^)(\s+)(?=[\w])", "", states)
    states = re.sub(r"(?<=[\w])(\s+)(?:$)", "", states)
    states = re.sub(r"(?<=[\w])(\s+)(?=,)", "", states)
    states = re.sub(r"(?<=,)(\s+)(?=[\w])", "", states)

    # kontrola syntaxe stavu
    validator("((?<!_)([a-zA-Z0-9]+(?:[\w]*[a-zA-Z0-9]+)*)(?:,|$))+", states)

    # rozdeleni na jednotlive stavy do mnoziny
    states = set(states.split(","))


# VSTUPNI SYMBOLY:

# mnozina vstupnich symbolu nesmi byt prazdna...
if len(input_symbols) == 0:
    print("ERR: Vstupni automat nesplnuje popsana lexikalni a syntakticka pravidla!", file=sys.stderr)
    exit(41)
else:
    # odstraneni prebytechnych mezer
    input_symbols = re.sub(r"(?:^)(\s+)(?=')", "", input_symbols)
    input_symbols = re.sub(r"(?<=')(\s+)(?:$)", "", input_symbols)
    input_symbols = re.sub(r"(?<=')(\s+)(?!')", "", input_symbols)
    input_symbols = re.sub(r"(?<=,)(\s+)(?=')", "", input_symbols)

    # kontrola syntaxe vstupnich symbolu
    validator("((([']{4})|(['][^'][']))(?:,|$))+", input_symbols)

    # rozdeleni na jednotlive vstupni symboly do mnoziny
    input_symbols = set(re.findall("((?:[']{4})|(?:['][^'][']))(?:,|$)", input_symbols))

# PRAVIDLA:
if len(rules) != 0:
    # odstraneni prebytechnych mezer
    rules = re.sub(r"(?:^)(\s+)(?=[\w])", "", rules)
    rules = re.sub(r"(?<=,)(\s+)(?=[\w])", "", rules)
    rules = re.sub(r"(?<=[\w])(\s+)(?=[,'])", "", rules)
    rules = re.sub(r"(?<=['\w])(\s+)(?=-)", "", rules)
    rules = re.sub(r"(?<=>)(\s+)(?=[\w])", "", rules)
    rules = re.sub(r"(?<=[\w])(\s+)(?:$)", "", rules)

    # kontrola syntaxe pravidel
    validator(
        "(?<!_)(([a-zA-Z]+(?:[\w]*[a-zA-Z0-9]+)*(([']{4})|(['][^']?[']))?->[a-zA-Z]+(?:[\w]*[a-zA-Z0-9]+)*)(?:,|$))+",
        rules)

    rules = set(re.findall("(?<!_)([a-zA-Z]+(?:[\w]*[a-zA-Z0-9]+)*(?:(?:[']{4})|(?:['][^']?[']))?->[a-zA-Z]+(?:[\w]*[a-zA-Z0-9]+)*)(?:,|$)", rules))

# VYCHOZI STAV:
validator("(?<!_)([a-zA-Z]+(?:[\w]*[a-zA-Z0-9]+)*)", start_state)

if start_state not in states:
    print("ERR: Semanticka chyba!", file=sys.stderr)
    exit(41)

start_state = {start_state}


# KONECOVE STAVY:
if len(final_states) != 0:
    # odstraneni prebytechnych mezer
    final_states = re.sub(r"(?:^)(\s+)(?=[\w])", "", final_states)
    final_states = re.sub(r"(?<=[\w])(\s+)(?:$)", "", final_states)
    final_states = re.sub(r"(?<=[\w])(\s+)(?=,)", "", final_states)
    final_states = re.sub(r"(?<=,)(\s+)(?=[\w])", "", final_states)

    validator("((?<!_)([a-zA-Z]+(?:[\w]*[a-zA-Z0-9]+)*)(?:,|$))+", final_states)

    final_states = set(final_states.split(","))

########################################################################################################################
########################################################################################################################

# Vytvoreni konecneho automatu
fsm = my.KA()

fsm.create(states, input_symbols, rules, start_state, final_states)

if args.no_eps_rules or args.determinization or args.analyze_string:
    fsm = fsm.remove_epsilon_transitions()

if args.determinization or args.analyze_string:
    fsm = fsm.determinization()


if args.analyze_string:
    ret_val = fsm.str(args.analyze_string)
    if ret_val:
        print("1", end="", file=output_handler)
    else:
        print("0", end="", file=output_handler)
    exit()


if not fsm.list_of_rules():
    print("(\n{" + ", ".join(fsm.list_of_states()) + "},\n{" + ", ".join(fsm.list_of_input_symbols()) + "},\n{" +
          ",\n".join(fsm.list_of_rules()) + "\n},\n" + fsm.start_state().name + ",\n{" +
          ", ".join(fsm.list_of_final_state()) + "}\n)", end="", file=output_handler)
else:
    print("(\n{" + ", ".join(fsm.list_of_states()) + "},\n{" + ", ".join(fsm.list_of_input_symbols()) + "},\n{\n" +
          ",\n".join(fsm.list_of_rules()) + "\n},\n" + fsm.start_state().name + ",\n{" +
          ", ".join(fsm.list_of_final_state()) + "}\n)", end="", file=output_handler)




