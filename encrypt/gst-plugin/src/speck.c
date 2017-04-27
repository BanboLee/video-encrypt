#include "speck.h"
#include <stdio.h>

#define ROR(x, r) ((x >> r) | (x << (32 - r)))
#define ROL(x, r) ((x << r) | (x >> (32 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)
#define RR(x, y, k) (y ^= x, y = ROR(y, 3), x ^= k, x -= y, x = ROL(x, 8))


void key_schedule(KEYTYPE const k[static KEYLENGTH], KEYTYPE keys[static ROUNDS]) {
    KEYTYPE k1 = k[0];
    KEYTYPE k2 = k[1];
    KEYTYPE k3 = k[2];
    KEYTYPE l[ROUNDS + 1];

    keys[0] = k1;
    l[0] = k2;
    l[1] = k3;

    int i;
    for (i = 0; i < ROUNDS - 1; i++) {
        l[i + KEYLENGTH - 1] = (keys[i] + ROR(l[i], 8)) ^ i;
        keys[i + 1] = ROL(k[i], 3) ^ l[i + KEYLENGTH - 1];
    }
}

void encrypt(DATATYPE const pt[static 2], DATATYPE ct[static 2], KEYTYPE const keys[static ROUNDS]) {
    int i;
    ct[0] = pt[0];
    ct[1] = pt[1];
    // printf("Start encrpty...  ct[0] = 0x%08x, ct[1] = 0x%08x\n", ct[0], ct[1]);
    for (i = 0; i < ROUNDS; i++) {
        R(ct[0], ct[1], keys[i]);
        // printf("Rounds: %d, keys = 0x%08x, ct[0] = 0x%08x, ct[1] = 0x%08x\n", i, keys[i], ct[0], ct[1]);
    }
}

void decrypt(DATATYPE const ct[static 2], DATATYPE pt[static 2], KEYTYPE const keys[static ROUNDS]) {
    int i;
    pt[0] = ct[0];
    pt[1] = ct[1];
    // printf("Start decrpty...  pt[0] = 0x%08x, pt[1] = 0x%08x\n ", pt[0], pt[1]);
    for (i = ROUNDS -1; i >= 0; i--) {
        RR(pt[0], pt[1], keys[i]);
        // printf("Rounds: %d, keys = 0x%08x, pt[0] = 0x%08x, pt[1] = 0x%08x\n", i, keys[i], pt[0], pt[1]);
    }
}