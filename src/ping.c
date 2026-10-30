#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>

#include <stdio.h>

#include "icmp.h"
#include "ping.h"

#define MAXCOUNT 2
#define MAXBYTES 64

/* inspect a packet */
void inspect(const struct icmp_packet*);

void
ping(const char *addr)
{
	int i, s, d;
	struct addrinfo hints, *res, *rp;
	struct icmp_packet packet;
	ssize_t bytes, total_sent, total_recv;
	char buf[MAXBYTES];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	/* identify the address */
	if ((s = getaddrinfo(addr, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	/* get a socket and establish a connection */
	for (rp = res; rp != NULL; rp = rp->ai_next) {
		if ((d = socket(rp->ai_family, rp->ai_socktype,
			rp->ai_protocol)) == -1)
			continue;

		if (connect(d, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(d);
	}

	freeaddrinfo(res);

	/* connect fails if there is no file descriptor */
	if (d == -1) {
		fprintf(stderr, "connect: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* send data to the address */
	printf("PING %s (%s) Sending %zu bytes of data\n",
		rp->ai_canonname, addr, sizeof(packet));

	total_sent = 0, total_recv = 0;
	for (i = 0; i < MAXCOUNT; i++) {
		icmp_echo_message(&packet);

		bytes = sendto(d, &packet, sizeof(packet), 0,
			rp->ai_addr, rp->ai_addrlen);

		if (bytes == -1) {
			fprintf(stderr, "sendto: %s\n",
				strerror(errno));
			close(d);
			exit(EXIT_FAILURE);
		}

		total_sent += bytes;
		bytes = recvfrom(d, buf, MAXBYTES, 0, rp->ai_addr,
			&rp->ai_addrlen);

		if (bytes == -1) {
			fprintf(stderr, "recvfrom: %s\n",
				strerror(errno));
			close(d);
			exit(EXIT_FAILURE);
		}

		total_recv += bytes;
		printf("Received %zu bytes from %s: icmp_seq=%u\n",
			bytes, addr, packet.seq);
	}

	printf("\n--- %s ping statistics ---\n", addr);
	printf("%lu packets transmitted, %lu received",
		total_sent / sizeof(packet),
		total_recv / bytes);
	printf(", %.0f%% packet loss\n",
		(1 - (((double)total_sent / sizeof(packet)) /
		(total_recv / bytes))) * 100);
	close(d);
}

void
inspect(const struct icmp_packet *packet)
{
	printf("\n--- PACKET %u ---\n", packet->ident);
	printf("Type: %u\n", packet->type);
	printf("Code: %u\n", packet->code);
	printf("Checksum: %#06x (%u)\n",
		packet->checksum, packet->checksum);
	printf("Identifier: %u\n", packet->ident);
	printf("Sequence: %u\n", packet->seq);
	printf("Data: %#06x (%u)\n",
		packet->data, packet->data);
	printf("Size: %zu bytes\n", sizeof(*packet));
}
