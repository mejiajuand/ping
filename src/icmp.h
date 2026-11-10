// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ICMP_H
#define ICMP_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

struct icmp_packet {
	uint8_t type;
	uint8_t code;
	uint16_t sum;
	uint16_t ident;
	uint16_t seq;
	uint32_t data;
};

/* echo message constructor */
struct icmp_packet *icmp_echo_message(void);

/* inspect an icmp packet */
void icmp_inspect(const struct icmp_packet*);

#endif  /* ICMP_H */
