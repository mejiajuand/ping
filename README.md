# Ping

## About

This project is my introduction to network programming. I have implemented the
`ping` network utility. It issues out 4 packets and displays some statistics
concerning the packets. The number of packets transmitted can be controlled via
the `COUNT` macro in `ping.c`. This utility will only work on Unix-like systems, so that is
the BSD operating systems, the Macintosh operating system, and Linux systems.

## How to build

Navigate to the `src` directory and modify the `CC` variable in the `Makefile`
to point to your C compiler, either `gcc` or `clang`, then
run `make release` to compile an optimised executable. Then, if you are on
Linux, navigate to `./bin/release` and run the command
`sudo setcap cap_net_raw+ep ./ping` to grant the `CAP_NET_RAW` privilege to the
program. Note the full-stop forward-slash in `./ping`. 

**If the compiler flags in the Makefile give you problems**, compile it with
`cc main.c ping.c icmp.c -o ping -lm` where `cc` points to `gcc` or `clang`.

The BSD operating systems and the Macintosh operating system do not provide an
equivalent to `CAP_NET_RAW`, you will have to run the program as root.

The `CAP_NET_RAW` privilege is important because the program needs to open a
raw socket to transmit ICMP packets. I could not use datagram sockets to
implement `ping`, it appears that not all systems support it. `ping` is
traditionally implemented using raw sockets.

## How to run

Navigate to `./bin/release` and run the program using `./ping [ip_address]`.
If you are on a BSD or Macintosh system, run the program with root privileges.

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
