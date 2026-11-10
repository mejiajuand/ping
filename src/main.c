#include "ping.h"

int
main(int argc, char **argv)
{
	if (argc < 2) {
		/* IPv4 loopback address */
		ping("127.0.0.1");
	}
	else if (argc == 2) {
		ping(argv[1]);
	}
	else {
		return 1;
	}
	return 0;
}
