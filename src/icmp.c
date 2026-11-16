/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2026 Juan Mejia
 */

#include "icmp.h"

/* icmp identifier */
static uint16_t identifier = 0;

/* icmp sequence */
static uint16_t sequence = 0;

/* computes the Internet checksum */
static uint16_t checksum(const struct icmp_packet*);

struct icmp_packet *
icmp_echo_message(void)
{
	struct icmp_packet *packet = malloc(sizeof(struct icmp_packet));
	packet->type = 8; /* echo message */
	packet->code = 0;
	packet->sum  = 0;
	packet->ident = ++identifier;
	packet->seq   = ++sequence;
	packet->data  = 0x41414141;
	packet->sum = checksum(packet);
	return packet;
}

void
inspect(const struct icmp_packet *packet)
{
	printf("-- Inspecting Packet %u --\n", packet->ident);
	printf("ICMP Type: %u\n", packet->type);
	printf("ICMP Code: %u\n", packet->code);
	printf("ICMP Checksum: %#06x\n", packet->sum);
	printf("ICMP Identifier: %u\n", packet->ident);
	printf("ICMP Sequence: %u\n", packet->seq);
	printf("Data: %#06x\n", packet->data);
	printf("Size: %zu bytes\n", sizeof *packet);
}

static uint16_t
checksum(const struct icmp_packet *packet)
{
	uint32_t sum = packet->data + packet->ident + packet->seq
		+ packet->sum + packet->code + packet->type;
	sum = ~((sum & 0xffff) + (sum >> 16));
	return (uint16_t)sum;
}
