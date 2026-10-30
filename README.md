# Ping

## About

This project is my introduction to network programming. I'm
implementing the `ping` command in C. Currently, the program will only
run on UNIX systems. This is also my first time doing socket
programming.

## How to build

Run `make` inside the `src` folder.

## How to run

Use either `sudo ./ping [target]` from within the `bin` directory, or
`sudo make run` from within the `src` directory. Root privileges are
needed for this program because I am using raw sockets instead of
datagram or stream sockets. I needed to use raw sockets for this
low-level implementation.

## How to test

You can capture the packet using Wireshark or tcpdump and inspect it.
This is how I've been testing and checking my implementation. You can
also call the `inspect` function in `ping.c` and see the fields and
compare it to what you captured in Wireshark/tcpdump.

## Comments

I think by this point, I can say I am finished with the project. I do
not want to prolong it, this is the best I can do for now. I will
continue studying network programming and will work on other projects.

## Warning

I am not sure about the security of my program. Due to the fact that
you need to run it with root privileges, I would not run this program.
I'll check the security later, as well as run it through a debugger and
fix up any issues I encounter. That will be my next and last update
for this project.
