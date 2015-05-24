__author__ = 'xpiste04@stud.fit.vutbr.cz'

import re
from scapy.layers.inet import TCP


class PacketList(list):
    """
        Seznam paketu do ktereho se prida pouze paket ktery splnuje pozadavky
        specifikovane parametry
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    """

    _requestRegex = None
    _responseRegex = None

    def __init__(self, port, hdrs=None, response=False):
        super(PacketList, self).__init__()

        self._port = port
        self._hdrs = [x.lower() for x in hdrs]
        self._response = response

        self._requestRegex = re.compile('^(GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE) '
                                        '.+ HTTP/\d.\d.*$')

        self._responseRegex = re.compile(r"^HTTP/\d\.\d \d\d\d .+?$")

    def append(self, pkt):

        try:
            load = pkt.load[:pkt.load.index('\r\n')]

            if str(pkt['TCP'].dport) in self._port:

                if self._response is False:

                    if self._requestRegex.match(load):
                        list.append(self, HTTPRequest(pkt, self._hdrs))
                else:
                    if self._responseRegex.match(load):
                        list.append(self, HTTPResponse(pkt, self._hdrs))

        except:
            pass


class MyPacket(dict):
    """
        Obecna trida pro paket od ktere dedi HTTPResponse a HTTPRequest
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    """

    _hdrs = None

    def __init__(self, pkt):
        super(MyPacket, self).__init__()

        # test zda je v paketu IPv4 nebo IPv6
        ip = pkt.haslayer('IP') and 'IP' or 'IPv6'

        self.update({
            "port_src": pkt['TCP'].sport,
            "port_dst": pkt['TCP'].dport,
            "ip_src": pkt[ip].src,
            "ip_dst": pkt[ip].dst
        })

        self._do_dissect(pkt.load)

    def _do_dissect(self, payload):
        string = payload[payload.index("\r\n"):]
        vset = set(string.split("\r\n"))

        for value in vset:

            try:
                index = value.index(":")
                # a = retezec po dvojtecku...
                a = value[:index]
                # b = retezec od dvojtecky (+ 2) dal...
                b = value[index + 2:]

                if self._validator(a):
                    self[a] = b

            except ValueError:
                continue

    def _validator(self, a):
        return a.lower() in self._hdrs


class HTTPRequest(MyPacket):
    """
        Trida pro HTTP pozadavek
        ~~~~~~~~~~~~~~~~~~~~~~~~
    """

    _hdrs = None

    def __init__(self, pkt, hdrs):
        self._hdrs = hdrs
        MyPacket.__init__(self, pkt)


class HTTPResponse(MyPacket):
    """
        Trida pro HTTP odpoved
        ~~~~~~~~~~~~~~~~~~~~~~
    """

    _hdrs = None

    def __init__(self, pkt, hdrs):
        self._hdrs = hdrs
        MyPacket.__init__(self, pkt)


