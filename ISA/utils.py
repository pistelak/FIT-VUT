__author__ = 'xpiste04@stud.fit.vutbr.cz'

# potlaceni chybove hlasky scapy
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)

import scapy.all as scapy
from os import access, R_OK
import xml.etree.ElementTree as ET

from HTTPpacket import PacketList


class Sniffer(object):
    """
        Obecna trida pro sniffer implementuje vsechny potrebne funkce
        krome nacitani a kontroly vstupu to zajistuji oddedene tridy
        SniffFile a SniffIface
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    """

    _list = None
    _source = None
    _hdrs = None

    @property
    def source(self):
        return self._source

    def __init__(self, source, port, hdrs, response=False):

        if self.check_source(source):
            self._source = source
        else:
            print('ERR: Vstup nenalezen!')
            exit(1)

        self._hdrs = hdrs.split(',')
        self._port = port.split(',')
        self._list = PacketList(self._port, self._hdrs, response)

    def check_source(self, source):
        raise NotImplementedError('Metoda \'check_source\' '
                                  'musi byt implementovana!')

    def get_input(self):
        raise NotImplementedError('Metoda \'get_input\''
                                  ' musi byt implemetovana')

    def save_to_xml(self, output_file):
        xml = self._create_httphdrs()
        with open(output_file, 'w') as f:
            indent(xml)
            ET.ElementTree(xml).write(f)

    def _create_httphdrs(self):
        httphdrs = ET.Element('httphdrs')

        ip_block_generator = self._create_ip()

        for i in ip_block_generator:
            httphdrs.append(i)

        return httphdrs

    def _create_ip(self):
        """
            Jednoduchy generator za pouziti yield... :)
        """

        # jako prni si vytvorime mnozinu vsech zdrojovych IP adres v seznamu
        # paketu, ktere jsme zachytili/nacetli
        ip_address = {x['ip_src'] for x in self._list}
        # to nam zajisti, ze budeme mit kazdou ip adresu jednou, ale
        # potrebujeme nejakou strukturu, ktera se da indexovat:
        ip_address = list(ip_address)

        for i in range(ip_address.__len__()):

            # vytvorime blok pro konkretni ip adresu
            ip = ET.Element('ip', addr=ip_address[i])

            # pak si pro kazdou z IP adres vyfiltrujeme pakety, ktere k ni patri
            pkts_list = filter(lambda x: x['ip_src'] == ip_address[i], self._list)

            # a vsechny tyto pakety zapiseme do xml...
            for j in range(pkts_list.__len__()):
                ip.append(self._create_connection(self._list[j]))

            # po kazde zapsane IP adrese vratime rizeni metode _create_httphdrs,
            # ktera si ji ulozi
            yield ip

    def _create_connection(self, pkt):

        connection = ET.Element('connection', port=str(pkt['port_src']))

        for i in range(self._hdrs.__len__()):
            try:
                # hlavicka ze seznamu def. uzivatelem
                name = str(self._hdrs[i])

                # musim zmensit obsah slovniku kvuli case insensitive pozadavku
                # efektivita-- :(
                # duvod: snaha o zanechani stejne velikosti pismen jako v parametru
                #        i ve vypise...
                pkt_lower = dict((k.lower(), v) for k, v in pkt.items())

                # hodnota
                value = str(pkt_lower[name.lower()])
                new = ET.Element('header', name=name, value=value)
                connection.append(new)
            except:
                pass

        return connection


class SniffFile(Sniffer):
    """
        Vstupni data budou prichazet z pcap souboru
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    """

    def check_source(self, source):
        return access(source, R_OK)

    def get_input(self):
        pkts = scapy.rdpcap(self.source)

        for pkt in pkts:
            self._list.append(pkt)


class SniffIface(Sniffer):
    """
        Vstupni data budeme ziskavat pres nektere rozhrani
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    """

    def check_source(self, source):
        return source in scapy.conf.iface

    def get_input(self):
        scapy.sniff(filter='tcp', iface=self.source,
                    prn=lambda x: self._list.append(x))


def indent(elem, level=0):
    """
        copy and paste from http://effbot.org/zone/element-lib.htm#prettyprint
        it basically walks your tree and adds spaces and newlines so the tree is
        printed in a nice way
    """

    i = "\n" + level * "  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level + 1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i