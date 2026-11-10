// SPDX-License-Identifier: GPL-3.0-or-later

#include "icmp.h"

/* packet identifier */
static uint16_t identifier = 0;

/* packet sequence */
static uint16_t sequence = 0;

/* internet checksum */
uint16_t checksum(const struct icmp_packet*);

struct icmp_packet *
icmp_echo_message(void)
{
struct icmp_packet *packet = malloc(sizeof(struct icmp_packet));
	packet->type  = 8;  /* echo message */
	packet->code  = 0;
	packet->sum   = 0;
	packet->ident = ++identifier;
	packet->seq   = ++sequence;
	packet->data  = 0x41414141;  /* AAAA */
	packet->sum   = checksum(packet);
	return packet;
}

void
icmp_inspect(const struct icmp_packet *packet)
{
	printf("--- PACKET %u ---\n", packet->ident);
	printf("Type: %u\n", packet->type);
	printf("Code: %u\n", packet->code);
	printf("Checksum: %u\n", packet->sum);
	printf("Identifier: %u\n", packet->ident);
	printf("Sequence: %u\n", packet->seq);
	printf("Data: %#06x\n", packet->data);
	printf("Size: %zu\n", sizeof(*packet));
}

uint16_t
checksum(const struct icmp_packet *packet)
{
	uint32_t sum = packet->type + packet->code + packet->sum
		+ packet->ident + packet->seq + packet->data;
	sum = ~((sum & 0xffff) + (sum >> 16));
	return (uint16_t)sum;
}
