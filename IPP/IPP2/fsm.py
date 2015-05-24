#!/usr/bin/python3.2

#DKA:xpiste04

import re
import sys


class KA:
    def __init__(self):
        self.__arrayState = set()
        self.__inputSymbols = set()

    def create(self, states, input_symbols, rules, start_state, finish_states):
        """ Vytvori petici dle z mnozin predanych v argumentech"""
        self.add_input_symbols(input_symbols)

        # nacteni stavu
        for name in states:
            if name in start_state and name in finish_states:
                self.new_state(State(True, True, name))
            elif name in start_state:
                self.new_state(State(False, True, name))
            elif name in finish_states:
                self.new_state(State(True, False, name))
            else:
                self.new_state(State(False, False, name))

        # nacteni pravidel
        for rule in rules:
            if not self.new_rule(rule):
                print("ERR: Semanticka chyba!", file=sys.stderr)
                exit(41)

    def add_input_symbols(self, input_symbols):
        """ Nastavi vstupni abecedu. """
        self.__inputSymbols = input_symbols

    def new_state(self, state):
        """ Prida novy stav. """
        self.__arrayState.add(state)

    def new_rule(self, rule):
        """ Prida nove pravidlo. """
        match = re.match("([^'-]+)('.*')*->(.+)", rule, re.DOTALL)

        if match.group(2) not in self.__inputSymbols and '':
            return False

        for default_state in self.__arrayState:
            if default_state.name == match.group(1):
                break
        else:
            return False

        for end_state in self.__arrayState:
            if end_state.name == match.group(3):
                break
        else:
            return False

        default_state.add_rule(match.group(2), end_state)

        return True

    def remove_epsilon_transitions(self):
        """ Provede odstraneni epsilon pravidel.
            Vraci objekt typu KA. """
        rules = set()
        final_states = set()

        for state in self.__arrayState:

            eps_closure = state.eps_closure()

            for state_from_eps in eps_closure:
                for symbol in self.__inputSymbols:
                    if state_from_eps.get_next_states(symbol) is None:
                        continue
                    else:
                        rules |= {state.name + symbol + "->" + x.name for x in state_from_eps.get_next_states(symbol)}

            if set(self.list_of_final_state()) & set(eps_closure):
                final_states |= {state.name}

        new_fsm = KA()
        new_fsm.create(self.list_of_states(), self.list_of_input_symbols(),
                       rules, self.start_state().name, final_states)

        return new_fsm

    def determinization(self):
        """ Provede determinizace KA.
            Vraci objekt typu KA. """
        new_states = MySet()
        det_states = set()
        det_rules = set()
        det_final_states = set()

        new_states.add(self.start_state().name)

        while True:
            tmp_state = new_states.pop(0)
            det_states |= {tmp_state}

            for state in self.__arrayState:
                if state in tmp_state.split("_"):
                    for symbol in self.__inputSymbols:

                        tmp = set()
                        for i in {x for x in self.__arrayState if x in tmp_state.split("_")}:
                            next_states = i.get_next_states(symbol)
                            if next_states:
                                tmp |= set(next_states)

                        if not tmp:
                            continue

                        tmp = {x.name for x in tmp}
                        tmp = "_".join(sorted(tmp))

                        det_rules.add(tmp_state + symbol + "->" + tmp)

                        if tmp not in det_states:
                            new_states.add(tmp)

                        if set(self.list_of_final_state()).intersection(tmp.split("_")):
                            det_final_states |= {tmp}

            if not new_states:
                break

        new_fsm = KA()
        new_fsm.create(det_states, self.list_of_input_symbols(), det_rules,
                       self.start_state().name, det_final_states)

        return new_fsm

    def list_of_states(self):
        """ Vraci mnozinu vsech stavu. """
        return sorted({state.name for state in self.__arrayState})

    def list_of_input_symbols(self):
        """ Vraci mnozinu vstupnich symbolu. """
        return sorted(self.__inputSymbols)

    def list_of_rules(self):
        """ Vraci mnozinu vsech pravidel. """
        tmp = set()
        for state in self.__arrayState:
            rules = state.get_rules_for_print()
            if rules == '':
                continue
            else:
                rules = rules.split(", ")
                tmp.update(rules)
        return sorted(tmp)

    def start_state(self):
        """ Vraci pocetecni stav. """
        for state in self.__arrayState:
            if state.is_start():
                return state

    def list_of_final_state(self):
        """ Vraci mnozinu koncovych stavu. """
        return sorted({state.name for state in self.__arrayState if state.is_final()})

    def str(self, string):
        """ Rozsireni str """
        actual_state = self.start_state()

        for char in string:
            next_state = actual_state.get_next_states("'" + char + "'")

            if not next_state:
                return False
            else:
                actual_state = next_state.pop()

        if not actual_state.is_final():
            return False

        return True


class State:
    """
        Třída reprezentujici jeden stav konecneho automatu.
    """

    def __init__(self, isfinal, isstart, name):
        self.__isFinal = isfinal
        self.__isStart = isstart
        self.name = name
        self.__arrayRules = set()

    def __hash__(self):
        return hash(self.name)

    def __eq__(self, other):
        return self.__hash__() == other.__hash__()

    def __repr__(self):
        return self.name

    def final(self):
        """ Nastavi stav jako koncovy. """
        self.__isFinal = True

    def is_final(self):
        """ Vrati true pokud se jedna o koncovy stav. """
        return self.__isFinal

    def is_start(self):
        """ Vrati true pokud se jedna o pocatecni stav. """
        return self.__isStart

    def eps_closure(self):
        """ Vytvori epsilon uzaver nad aktualnim stavem. """
        eps_closure = MySet()
        eps_closure.add(self)

        for item in eps_closure:
            eps_closure.update(item.get_next_states("\'\'"))

        return eps_closure

    def add_rule(self, symbol, next_state):
        """ Prida pravidlo. """
        if not self.get_next_states(symbol):
            self.__arrayRules.add(Rule(self, symbol, next_state))
        else:
            for rule in self.__arrayRules:
                if rule.get_symbol() == symbol:
                    rule.add_end_state(next_state)
                    return

    def get_rules_for_print(self):
        """ Prevede vsechna pravidla vychazejici z daneho stavu do tvaru urcenemu pro tisk. """
        tmp = set()
        for rule in self.__arrayRules:
            tmp.add(', '.join(rule.for_print()))
        return ', '.join(tmp)

    def get_next_states(self, input_symbol):
        """ Vrati stavy do kterych se lze dostat z tohoto stavu pres parametr input. """
        for rule in self.__arrayRules:
            if rule.get_symbol() == input_symbol:
                return list(rule.get_finish_state())
        return None


class Rule:
    """
        Trida reprezentujici pravidlo.
        Pravidlo muze mit vice konecnych stavu ulozenych v mnozine.
    """

    def __init__(self, default, symbol, finish):
        self.__default = default
        self.__finish = set()
        self.__finish.add(finish)
        if symbol is None:
            self.__symbol = "\'\'"
        else:
            self.__symbol = symbol

    def __hash__(self):
        return hash(self.__symbol)

    def __eq__(self, other):
        return self.__hash__() == other.__hash__()

    def __repr__(self):
        """ Textova reprezentace pravidla - lip se v tom orientuje v debuggeru :D """
        return self.__default.name + self.__symbol + "->" + "|".join({x.name for x in self.__finish})

    def get_default_state(self):
        """ Vraci vychozi stav"""
        return self.__default

    def get_symbol(self):
        """ Vraci symbol pres ktery se pravidlo dostane do pro nej koncoveho stavu."""
        return self.__symbol

    def get_finish_state(self):
        """ Vraci stav/stavy (mnozinu - ne odkaz!) ktery je pro dane pravidlo koncovy."""
        return self.__finish

    def add_end_state(self, end_state):
        """ Prida dalsi koncovy stav k jiz existujicimu pravidlu. """
        self.__finish.add(end_state)

    def return_rule_mod(self):
        """ Pouziva se pri odstraneni epsilon prechodu - vraci mnozinu
            pravidel ve tvaru 'symbol->koncovy_stav' """
        tmp = set()
        for end_state in self.__finish:
            tmp.add(self.__symbol + '->' + end_state.name)
        return tmp

    def for_print(self):
        """ Vraci mnozinu obsahujici pravidlo/pravidla ve formatu pro tisk """
        tmp = set()
        for end_state in self.__finish:
            tmp.add(self.__default.name + ' ' + self.__symbol + ' -> ' + end_state.name)
        return sorted(tmp)


class MySet(list):
    """
        Trida postavena nad seznamem - poradi dle vlozeni a bez duplicit.
    """

    def __init__(self):
        super().__init__()

    def add(self, item):
        if item not in self:
            self.append(item)

    def update(self, src):
        while src:
            self.add(src.pop())

