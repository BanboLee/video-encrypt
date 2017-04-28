/*
** speck cipher algorithm implement
** words size: 32bits ,data block size: 2 words 64bits, key length: 3, key size: 3 words 96bits
** rounds: 26
*/


#ifndef SPECK_H
#define SPECK_H

#include <stdint.h>

#define ROUNDS 26
#define KEYLENGTH 3
#define KEYTYPE uint32_t
#define DATATYPE uint32_t

void key_schedule(KEYTYPE const iv[static KEYLENGTH], KEYTYPE keys[static ROUNDS]);
void encrypt(DATATYPE const pt[static 2], DATATYPE ct[static 2], KEYTYPE const keys[static ROUNDS]);
void decrypt(DATATYPE const ct[static 2], DATATYPE pt[static 2], KEYTYPE const keys[static ROUNDS]);

#endif
