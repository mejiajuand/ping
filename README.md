# Ping

## About

This project is my introduction to network programming. I have implemented the
`ping` network utility. It issues out 4 packets and displays some statistics
concerning the packets. The number of packets transmitted can be controlled via
the `COUNT` macro in `ping.c`. This utility works on both IPv4 and IPv6
addresses. This utility will only work on Unix-like systems, so that is
the BSD operating systems, the Macintosh operating system, and Linux systems.

## How to build

From within the `src` directory, run `make release` to compile an optimised
executable. Then, if you are on Linux, navigate to `./bin/release` and run the
command `sudo setcap cap_net_raw+ep ./ping` to grant the `CAP_NET_RAW`
privilege to the program. Note the full-stop forward-slash in `./ping`.

The BSD operating systems and the Macintosh operating system do not provide an
equivalent to `CAP_NET_RAW`, you will have to run the program as root.

The `CAP_NET_RAW` privilege is important because the program needs to open a
raw socket to transmit ICMP packets. I could not use datagram sockets to
implement `ping`. `ping` is traditionally implemented using raw sockets.

## How to run

Navigate to `./bin/release` and run the program using `./ping [target_ip]`.
If you are on a BSD or Mac system, run `./ping` with root privileges.
