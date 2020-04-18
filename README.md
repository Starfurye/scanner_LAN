# Simple Port Scanner on LAN

This scanner is only available on Linux, do not try to run it on Windows and WSL cause the latter do not support physical network adapter.

Not recommended on other Unix distributions, they may have different definitions for protocol handlers.


## Building

1. make

```bash
$ make
```

2. clean files

```bash
$ make clean
```


## Usage


1. Scan all hosts on LAN, it uses `fping` to search hosts in LAN, search results is saved in `hosts.log`:

```bash
$ sudo ./scanner
```

2. Scan specific host with ip address:

```bash
$ sudo ./scanner -i [ip address]
$ sudo ./scanner -i 192.168.1.102
```

3. Scan specific hosts with ip address within [start port, end port]:

```bash
$ sudo ./scanner -i [ip address] [start port] [end port]
$ sudo ./scanner -i 192.168.1.102 251 996
```


## about default values

It's free to configure default values since this scanner is under development up to now.


In `common.c > getHosts(), line 30`


CIDR address for fping: `192.168.1.0/24`;


In `common.h, line 26, 27`:


default start port: `1`, default end port: `1024`.
