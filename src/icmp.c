#include <arpa/inet.h>

#include "icmp.h"

static uint16_t ident = 0;
static uint16_t seq = 0;

/* internet checksum */
uint16_t checksum(const struct icmp_packet*);

void
icmp_echo_message(struct icmp_packet *packet)
{
	packet->type = 8; /* echo message */
	packet->code = 0;
	packet->checksum = 0;
	packet->ident = ++ident;
	packet->seq = ++seq;
	packet->data = htonl(0x41414141); /* AAAA */
	packet->checksum = checksum(packet);
}

uint16_t
checksum(const struct icmp_packet *packet)
{
	uint32_t sum = packet->type + packet->code \
		+ packet->checksum + packet->ident \
		+ packet->seq + packet->data;
	sum = (sum & 0xffff) + (sum >> 16);
	return ~sum;
}
