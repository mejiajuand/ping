# Ping

## About

This project is my introduction to network programming. I'm implementing the
`ping` command in C. Currently, the program will only run on UNIX systems. This
is also my first time doing socket programming.

## How to build

Run `make` inside the `src` folder.

## How to run

Use either `sudo ./ping` from within the `bin` directory, or `sudo make run`
from within the `src` directory. Root privileges are needed for this program
because I am using raw sockets instead of datagram or stream sockets. I needed
to use raw sockets for this low-level implementation.

## How to test

You can capture the packet using Wireshark or tcpdump and inspect it. This is
how I've been testing and checking my implementation.

## Comments
This is still not complete. I managed to get the command to work and get a
reply back from the target. The code is a little sloppy though and could be
cleaned up and the `printf` output could be nicer. This is just my first
working version, more updates to come.
