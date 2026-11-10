# Ping

## About

This project is my introduction to network programming. I have implemented the
`ping` network utility. It issues out 4 packets and displays some statistics
concerning the packets. The number of packets transmitted can be controlled via
the `COUNT` macro in `ping.c`. This utility works on both IPv4 and IPv6
addresses.

## How to build

From within the `src` directory, run `make release` to compile an optimised
executable. Then, navigate to `./bin/release` and run the command
`sudo setcap cap_net_raw+ep ./ping` to grant the `CAP_NET_RAW` privileges to
the program. Note the full-stop forward-slash in `./ping`.

The `setcap` line is important because the program needs to open raw sockets
to transmit ICMP packets. I could not use datagram sockets to implement `ping`,
nor could I find a workaround. It appears that `ping` is traditionally
implemented using raw sockets.

## How to run

Navigate to `./bin/release` and run the program using `./ping [target_ip]`.
