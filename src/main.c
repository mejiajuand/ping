#include "ping.h"

int
main(int argc, char *argv[])
{
	if (argc != 2)
		ping("127.0.0.1");  /* loopback address */
	else
		ping(argv[1]);
	return 0;
}
