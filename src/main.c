#include <stdio.h>

#include "ping.h"

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: ./ping <ip_address>\n");
		return 1;
	}

	ping(argv[1]);
	return 0;
}
