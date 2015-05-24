#!/usr/bin/python3.2

#DKA:xpiste04

import getopt
import sys


class Params:
    def __init__(self):
        self.help = False
        self.input = False
        self.output = False
        self.no_eps_rules = False
        self.determinization = False
        self.case_insensitive = False
        self.analyze_string = False

    @staticmethod
    def print_help():
        """ Vypise napovedu na stdout. """
        print("""
            DKA - Determinizace konecneho automatu:
                --help                   tisk napovedy
                --input=file             vstupni soubor | stdin
                --output=file            vystupni soubor | stdout
                -e, --no-epsilon-rules   odstraneni epsilon prechodu
                -d, --determizization    provede determinizaci
                -i, --case-insensitiv    nebere ohled na velikost znaku

                Radek Pištělák (c) xpiste04@stud.fit.vutbr.cz""")
        exit()

    @staticmethod
    def argparser_error():
        """ Ukonci program s chybou. """
        print("ERR: Chyba pri zpracovani parametru!! Pro napovedu: --help", file=sys.stderr)
        exit(1)

    def parse(self):
        """ Rozparsuje argv na parametry progrmau. """
        try:
            opts, args = getopt.getopt(sys.argv[1:], "edi", ["help", "input=", "output=", "no-epsilon-rules",
                                                             "determinization", "case-insensitive", "analyze-string="])
        except getopt.GetoptError:
            self.argparser_error()

        for o, a in opts:

            if o == "--help":
                if len(sys.argv) != 2:
                    self.argparser_error()
                else:
                    self.print_help()
            elif o in ("-e", "--no-epsilon-rules"):
                if not self.no_eps_rules:
                    self.no_eps_rules = True
                else:
                    self.argparser_error()
            elif o in ("-d", "--determinization"):
                if not self.determinization:
                    self.determinization = True
                else:
                    self.argparser_error()
            elif o in ("-i", "--case-insensitive"):
                if not self.case_insensitive:
                    self.case_insensitive = True
                else:
                    self.argparser_error()
            elif o == "--input":
                if not self.input:
                    self.input = a
                else:
                    self.argparser_error()
            elif o == "--output":
                if not self.output:
                    self.output = a
                else:
                    self.argparser_error()
            elif o == "--analyze-string":
                if not self.analyze_string:
                    self.analyze_string = a
                else:
                    self.argparser_error()
            else:
                self.argparser_error()

        if self.determinization and self.no_eps_rules:
            self.argparser_error()

        return self


