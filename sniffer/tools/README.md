# RIOT Sniffer Application


## About

This sniffer script can be used to monitor and capture network traffic using
a RIOT based node. It is primarily designed for sniffing wireless data traffic,
but can also well be used for wired network traffic, as long as used network
devices support promiscuous mode and output of raw data.

The python script `sniffer.py` requires a RIOT node running the sniffer app, its
source code is located in this repository (see main folder). This node outputs
received network traffic via a serial port or a network socket in the common
Wireshark/libpcap (pcap) format. This output is then parsed by the `sniffer.py`
script included in this folder run on a host computer.

The `sniffer.py` script is a modified version of [malvira's script](https://github.com/malvira/libmc1322x/blob/master/tools/rftestrx2pcap.py)
for the Redbee Ecotag (https://github.com/malvira/libmc1322x/wiki/wireshark).

## Dependencies

The `sniffer.py` script is written in Python and needs [pyserial](https://pypi.python.org/pypi/pyserial).

Installing the dependencies:


#### Debuntu
    apt-get install python-serial

#### PIP
    pip install pyserial


## Usage

General usage:

1.  Flash an applicable RIOT node with the sniffer application 
```
$ make all flash
```

2.  Run the `sniffer.py` script (change to subfolder `tools/`) as follows :
    For serial port:
```
$ ./sniffer.py -b 115200 /dev/ttyUSB0 26
```

For detailed information on the parameters use the scripts on-line help:

```
./sniffer.py -h
```

### Examples

The following examples are made when using the sniffer application together with
an `iotlab-m3` node that is connected to `/dev/ttyUSB1`(or COM1) (`serial` connection type)
and runs per default with a baudrate of 500000. For the `socket` connection type port 20000
is used.

#### Linux (serial)

Dump packets to a file:
```
$ ./sniffer.py -b 115200 /dev/ttyUSB0 26 foo.pcap
```

This .pcap can then be opened in Wireshark.

Alternatively for live captures, you can pipe directly into Wireshark with:
```
$ ./sniffer.py -b 115200 /dev/ttyUSB0 26 | wireshark -k -i -
```
