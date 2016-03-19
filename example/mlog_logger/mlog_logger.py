#!/usr/bin/env python3

"""
MLOG logger -- A simple MLOG log message receiver.

Listens on localhost port 12350 for MLOG log messages and prints them in a
human-readable way.
"""

import collections
import os
import os.path
import re
import socket
import sys
import time

SERVER_MULTICAST_ADDRESS = socket.gethostbyname('localhost')
SERVER_PORT = 12350

VALUE_PRESENT_MASK = ((1 << 6) << 24)
READABLE_LOG_LEVELS = {"II": "INFO", "WW": "WARN", "EE": "ERROR"}

#pylint:disable=too-few-public-methods
class LogSource:
    def receive(self):
        raise NotImplementedError()

    @staticmethod
    def _parse(data):
        magic = b"MLOG"
        empty = True
        if data.startswith(magic):
            # version = data[4]
            # colon = data[5]
            payload = data[6:]

            if len(payload) > 0:
                empty = False
                word = 0
                for i, byte in enumerate(payload):
                    word <<= 8
                    word |= byte
                    if (i + 1) % 4 == 0:
                        yield word
                        word = 0
        # Indicate that an empty line has been detected.
        if empty:
            yield None

#pylint:disable=too-few-public-methods
class UdpLogSource(LogSource):
    def __init__(self, sock):
        LogSource.__init__(self)
        self.sock = sock

    def receive(self):
        while True:
            received_data = self.sock.recv(10240)
            t = time.time()
            # New packet, send None to sync.
            yield (t, None)
            for w in self._parse(received_data):
                if w != None:
                    yield (t, w)

#pylint:disable=too-few-public-methods
class LogInfoMapBuilder:
    PATTERN = re.compile(r"([0-9a-fA-F]+)\s+MLOG_(\w+)_(II|WW|EE)_(\w+)___")
    LogInfo = collections.namedtuple("LogInfo", "component level text address")

    def __init__(self):
        pass

    @staticmethod
    def build_log_info_map(mlog_map_file):
        log_info_map = {}
        with open(mlog_map_file, "r") as mlog_map_file:
            for line in mlog_map_file.readlines():
                log_info = LogInfoMapBuilder._parse_log_string(line.rstrip())
                if log_info:
                    log_info_map[log_info.address] = log_info
        return log_info_map

    @staticmethod
    def _parse_log_string(line):
        # 012f7e48 MLOG_PIA_CLIENT_ADAPTER_II_cyclic_task_time_is___
        m = LogInfoMapBuilder.PATTERN.fullmatch(line)
        if m:
            int_address = int(m.group(1), 16)
            return LogInfoMapBuilder.LogInfo(
                component=m.group(2), level=READABLE_LOG_LEVELS[m.group(3)], text=m.group(4), address=int_address)
        return None

def abort(text):
    sys.stderr.write(text + "\n")
    sys.exit(1)

def is_value_present(address):
    return address & VALUE_PRESENT_MASK

def plain_address(address):
    return (address & ~VALUE_PRESENT_MASK) & 0xFFFFFFFF

def main():
    if len(sys.argv) != 2:
        this_tool = os.path.basename(sys.argv[0])
        abort("Usage: " + this_tool + " <mlogmap-file>")
    mlog_map_file = sys.argv[1]

    # pylint:disable=broad-except
    try:
        log_info_map_builder = LogInfoMapBuilder()
        log_info_map = log_info_map_builder.build_log_info_map(mlog_map_file)
    except FileNotFoundError:
        abort("Cannot find map file")
    except Exception:
        abort("Map file processing failed")

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    sock.bind(('', SERVER_PORT))

    log_source = UdpLogSource(sock)
    time0 = time.time()

    receiver_generator = log_source.receive()
    for timestamp, word in receiver_generator:
        if word != None:
            time_delta = timestamp - time0
            address = word
            if is_value_present(address):
                address = plain_address(address)
                _, word = next(receiver_generator)
                if word != None:
                    int_value = int(word)
                    log_info = log_info_map.get(address)
                    if log_info:
                        print("%.3f %s %s %s 0x%X %d" % (time_delta, log_info.component, log_info.level, log_info.text, int_value, int_value))
            else:
                log_info = log_info_map.get(address)
                if log_info:
                    print("%.3f %s %s %s" % (time_delta, log_info.component, log_info.level, log_info.text))
        # Sync.
        else:
            # Send every line to stdout immediately (no buffering).
            sys.stdout.flush()


if __name__ == '__main__':
    main()

