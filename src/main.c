// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Juan Mejia

#include "ping.h"

int
main(int argc, char **argv)
{
	if (argc < 2) {
		ping("127.0.0.1");  /* loopback address */
	}
	else if (argc == 2) {
		ping(argv[1]);
	}
	else {
		return 1;
	}
	return 0;
}
