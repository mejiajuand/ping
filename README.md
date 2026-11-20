# Ping

## About

This project is my introduction to network programming. I have implemented the
`ping` network utility. It issues out 4 packets and displays some statistics
concerning the packets. The number of packets transmitted can be controlled via
the `COUNT` macro in `ping.c`. This utility will only work on Unix-like systems,
so that is the BSD operating systems, the Macintosh operating system, and Linux
systems. This utility should work on both IPv4 and IPv6 addresses.

## How to build

With either `gcc` or `clang` installed, run `make release`, then, if you are on
Linux, navigate to `./bin/release/` and give the executable the `CAP_NET_RAW` privilege by
running the following command: `sudo setcap cap_net_raw+ep ./ping`. Note the full-stop
forward-slash before the executable name. If you are on MacOS or BSD, you will need to
run the executable as root. MacOS and BSD do not have an equivalent to Linux's `CAP_NET_RAW`
privilege. This privilege is necessary because the program operates on **raw sockets**. I
could not implement this command using datagram sockets, it appears that this is not a
supported feature on many operating systems. `ping` is traditionally implemented
using **raw sockets**, and so it requires permissions to open raw sockets.

## How to run

Navigate to `./bin/release` and run the program using `./ping <ip_address>`.
If you are on a BSD or Macintosh system, run the program as root.

## References

- Hands-On Network Programming with C by Lewis Van Winkle
- Beej's Guide to Network Programming Using Internet Sockets by Brian "Beej Jorgensen" Hall
- RFC 791 - Internet Protocol
- RFC 792 - Internet Control Message Protocol
- RFC 1071 - Computing the Internet checksum
- RFC 1141 - Incremental updating of the Internet checksum
- Linux getaddrinfo(3) man page
- Linux socket(2) man page
- Linux connect(2) man page
- Linux sendto(2) man page
- Linux recvfrom(2) man page
- Linux errno(3) man page
- Linux malloc(3) man page
- Linux time.h(0P) man page
- Linux close(2) man page
- Linux memset(3) man page
- Linux size_t(3type) man page
- Linux tcpdump(1) man page
- Linux tshark(1) man page
