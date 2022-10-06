#ifndef SHA256_H
#define SHA256_H

#include <stdlib.h>
#include <memory.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SHA256_BLOCK_SIZE 32


typedef struct {
	uint8_t data[64];
	uint32_t datalen;
	unsigned long long bitlen;
	uint32_t state[8];
} sha256_ctx;

/* public functions */
void sha256(char *, char *);

#endif
