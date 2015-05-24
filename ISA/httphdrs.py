#!/usr/bin/python2.7

__author__ = 'xpiste04@stud.fit.vutbr.cz'

import argparse
import utils as my


parser = argparse.ArgumentParser()

group = parser.add_mutually_exclusive_group(required=True)
group.add_argument("-f", dest="file", metavar="FILE",
                   help="Pouzije jako vstup soubor ve formatu pcap.")
group.add_argument("-i", dest="iface",
                   help="Pouzije jako vstup rozhrani iface.")
parser.add_argument("-H", dest="headers", action="store", default=None,
                    help="Slouzi ke specifikaci sledovanych HTTP hlavicek.")
parser.add_argument("-p", dest="port", action="store", default=None,
                    help="Slouzu ke specifikaci portu (muze jich byt vice).")
parser.add_argument("-o", dest="output", required=True,
                    help="Povinny parametr specifikuje nazev vyst. souboru.")
parser.add_argument("--extra", dest="extra", action="store_true", default=False,
                    help="Rozsireni, ktere nebude ukladat HTTPRequest ale HTTPResponse")

args = parser.parse_args()


def get_input():
    """
         Jednoducha "tovarna" pro zjisteni odkud potecou vstupni data
    """

    source = None

    if args.port is None:
        args.port = "80"
    else:
        args.port = args.port.replace(" ", "")

    if args.headers is None:
        if args.extra is False:
            args.headers = "User-Agent,Accept,Accept-Encoding,Accept-Language"
        else:
            args.headers = "Host,Location,Date,Status-Line,Server"
    else:
        args.headers = args.headers.replace(" ", "")

    if args.iface:
        source = my.SniffIface(str(args.iface), port=args.port,
                               hdrs=args.headers, response=args.extra)
    elif args.file:
        source = my.SniffFile(str(args.file), port=args.port,
                              hdrs=args.headers, response=args.extra)

    return source


if __name__ == "__main__":
    # zjistim odkud budu brat vstupni data
    httphdrs = get_input()
    # nactu vstupni data
    httphdrs.get_input()
    # ulozim do xml
    httphdrs.save_to_xml(args.output)
    # koncim
