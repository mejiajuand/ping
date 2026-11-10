// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Juan Mejia

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "icmp.h"
#include "ping.h"

#define COUNT 4        /* ping count */
#define BUFFERSIZE 64

void
ping(const char addr[])
{
	int ec, d;  /* error code, file descriptor */
	struct addrinfo hints, *p;
	struct icmp_packet *packet;

	int i;
	char buf[BUFFERSIZE];
	time_t start, end;
	double elapsed, min = -1, max = 0, total_time = 0;
	ssize_t sent, received, total_sent = 0, total_received = 0;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_flags = AI_ADDRCONFIG;

	/* lookup and resolve the address */
	if ((ec = getaddrinfo(addr, NULL, &hints, &p)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ec));
		exit(EXIT_FAILURE);
	}

	/* retrieve a socket file descriptor */
	if ((d = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
		fprintf(stderr, "socket: %s\n", strerror(errno));
		goto CLEANUP; exit(EXIT_FAILURE);
	}

	/* establish a connection to the target */
	if ((ec = connect(d, p->ai_addr, p->ai_addrlen)) == -1) {
		fprintf(stderr, "connect: %s\n", strerror(errno));
		goto CLEANUP; exit(EXIT_FAILURE);
	}

	/* send the packet */
	printf("Pinging %s with %zu bytes of data\n", addr, sizeof(*packet));

	for (i = 0; i < COUNT; i++) {
		/* create the icmp packet */
		packet = icmp_echo_message();

		/* time the trip */
		start = time(NULL);

		sent = sendto(d, packet, sizeof(*packet), 0,
			p->ai_addr, p->ai_addrlen);

		if (sent == -1) {
			fprintf(stderr, "sendto: %s\n", strerror(errno));
			free(packet); goto CLEANUP; exit(EXIT_FAILURE);
		}

		received = recvfrom(d, buf, BUFFERSIZE, 0,
			p->ai_addr, &p->ai_addrlen);

		if (received == -1) {
			fprintf(stderr, "recvfrom: %s\n", strerror(errno));
			free(packet); goto CLEANUP; exit(EXIT_FAILURE);
		}

		/* end the trip timer */
		end = time(NULL);

		/* compute round-trip time */
		elapsed = difftime(end, start);

		/* set the minimum and maximum times */
		if (elapsed > max) {
			max = elapsed;
		}
		if (elapsed < min) {
			min = elapsed;
		}
		if (min < 0) {
			min = elapsed;
		}

		/* sum the totals */
		total_time += elapsed;
		total_sent += sent;
		total_received += received;

		/* display the ping result */
		printf("Received %zu bytes from %s: icmp_seq=%u time=%f s\n",
			received, addr, packet->seq, elapsed);

		free(packet);
	}

	/* display the ping statistics */
	printf("\n--- %s ping statistics ---\n", addr);
	printf("%zd packets sent, ", total_sent / sent);
	printf("%zd packets received, ", total_received / received);
	printf("%.0f%% packet loss, ", (1 - (double)(sent * total_received)
		/ (double)(received * total_sent)) * 100);
	printf("time %f s\n", total_time);

	/* display the round-trip time statistics */
	printf("rtt min/avg/max = %f/%f/%f s\n", min, (max + min) / 2, max);

CLEANUP:
	if (d != -1) {
		close(d);
	}

	if (p) {
		freeaddrinfo(p);
	}
}
