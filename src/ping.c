#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <math.h>

#include "ping.h"
#include "icmp.h"

#define MAXSIZE 64
#define COUNT 4

void
ping(const char node[])
{
	int ec, d = -1; /* error code, descriptor */
	struct addrinfo hints, *res, *rp;

	char buf[MAXSIZE];
	time_t start, end;
	struct icmp_packet *packet = NULL;
	double rtt, total_time = 0, min = -1, avg, max = 0, mdev;
	ssize_t sent, received, total_sent = 0, total_received = 0;

	memset(&hints, 0, sizeof hints);
	hints.ai_family   = PF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_flags    = AI_ADDRCONFIG;

	/* resolve the address */
	if ((ec = getaddrinfo(node, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ec));
		return;
	}

	/* attempt to connect to the address */
	for (rp = res; rp != NULL; rp = rp->ai_next) {
		if ((d = socket(rp->ai_family, rp->ai_socktype,
			rp->ai_protocol)) == -1) {
			continue;
		}

		if (connect(d, rp->ai_addr, rp->ai_addrlen) == -1) {
			close(d);
			continue;
		}

		break;

	}

	freeaddrinfo(res);

	/* no suitable address to connect to */
	if (rp == NULL) {
		fprintf(stderr, "connect: %s\n", strerror(errno));
		goto CLEANUP;
	}

	/* send a message to the host */
	printf("Pinging %s with %zu bytes of data\n", node,
		sizeof *packet);
	for (int i = 0; i < COUNT; i++) {
		packet = icmp_echo_message();

		start = time(NULL); /* time the round-trip */

		if ((sent = send(d, packet, sizeof *packet, 0)) == -1) {
			fprintf(stderr, "send: %s\n", strerror(errno));
			free(packet); goto CLEANUP;
		}

		if ((received = recv(d, buf, MAXSIZE, 0)) == -1) {
			fprintf(stderr, "recv: %s\n", strerror(errno));
			free(packet); goto CLEANUP;
		}

		end = time(NULL);

		/* calculate the round-trip time */
		rtt = difftime(end, start);

		/* record the minimum and maximum round-trip times */
		if (rtt < min || min == -1)
			min = rtt;

		if (rtt > max)
			max = rtt;

		/* sum the totals */
		total_time += rtt;
		total_sent += sent;
		total_received += received;

		/* print the ping statistics */
		printf("%zd bytes from %s: seq=%u time=%0.f ms\n",
			received, node, packet->seq, rtt * 1000);

		free(packet);
	}

	/* summarise the ping statistics and print */
	printf("\n-- %s ping statistics --\n", node);
	printf("packets sent: %u | ", (unsigned)(total_sent / sent));
	printf("packets received: %u | ",
		(unsigned)(total_received / received));
	printf("packet loss: %.0f%% | ",
		(1 - (double)(sent * total_received)
		/ (double)(received * total_sent)) * 100);
	printf("time: %.0f ms\n", total_time * 1000);

	/* summarise the rtt statistics and print */
	avg = (min + max) / 2;
	mdev = sqrt((pow(min, 2) + pow(max, 2)) / 2) - pow(avg, 2);
	printf("\n-- round-trip time statistics --\n");
	printf("rtt min/avg/max/mdev = %.0f/%.0f/%.0f/%.0f ms\n",
		min * 1000, avg * 1000, max * 1000, mdev * 1000);

CLEANUP:
	if (d != -1)
		close(d);
}
