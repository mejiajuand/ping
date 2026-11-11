// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Juan Mejia

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <math.h>

#include "icmp.h"
#include "ping.h"

#define COUNT 4
#define BUFFERSIZE 64

void
ping(const char addr[])
{
	int ec, d; /* error code, file descriptor */
	struct addrinfo hints, *p;

	time_t start, end;
	char buf[BUFFERSIZE];
	struct icmp_packet *packet;
	double rtt, total_rtt = 0, min = -1, avg, max = 0, mdev;
	ssize_t sent, received, total_sent = 0, total_received = 0;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_flags = AI_ADDRCONFIG;

	/* resolve the address */
	ec = getaddrinfo(addr, NULL, &hints, &p);

	if (ec != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ec));
		exit(EXIT_FAILURE);
	}

	/* create a socket */
	d = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

	if (d == -1) {
		fprintf(stderr, "socket: %s\n", strerror(errno));
		goto CLEANUP; exit(EXIT_FAILURE);
	}

	/* connect to the target */
	ec = connect(d, p->ai_addr, p->ai_addrlen);

	if (ec != 0) {
		fprintf(stderr, "connect: %s\n", strerror(errno));
		goto CLEANUP; exit(EXIT_FAILURE);
	}

	/* send the message to the target */
	printf("Pinging %s with %zu bytes of data\n",
		addr, sizeof *packet);

	for (int i = 0; i < COUNT; i++) {
		start = time(NULL);
		packet = icmp_echo_message();

		sent = send(d, packet, sizeof *packet, 0);

		if (sent == -1) {
			fprintf(stderr, "send: %s\n", strerror(errno));
			free(packet); goto CLEANUP; exit(EXIT_FAILURE);
		}

		received = recv(d, buf, BUFFERSIZE, 0);

		if (received == -1) {
			fprintf(stderr, "recv: %s\n", strerror(errno));
			free(packet); goto CLEANUP; exit(EXIT_FAILURE);
		}

		end = time(NULL);
		rtt = difftime(end, start);

		/* sum the total values */
		total_rtt += rtt;
		total_sent += sent;
		total_received += received;

		/* get the min and max times */
		if (rtt > max)
			max = rtt;
		if (rtt < min || min == -1)
			min = rtt;

		printf("%zd bytes received from %s: seq=%u time=%f s\n",
			received, addr, packet->seq, rtt);

		free(packet);
	}

	/* display ping statistics */
	printf("\n--- %s ping statistics ---\n", addr);
	printf("%zd packets sent, ", total_sent / sent);
	printf("%zd packets received, ", total_received / received);
	printf("%.0f%% packet loss, ",
		(1 - (double)(sent * total_received)
		/ (double)(received * total_sent)) * 100);
	printf("time %f s\n", total_rtt);

	/* display rtt statistics */
	avg = (min + max) / 2;
	mdev = sqrt((((min * min) + (max * max)) / 2) - (avg * avg));
	printf("\n--- round-trip time statistics ---\n");
	printf("rtt min/avg/max/mdev = %f/%f/%f/%f s\n",
		min, avg, max, mdev);

CLEANUP:
	if (d != -1)
		close(d);

	if (p)
		freeaddrinfo(p);
}
