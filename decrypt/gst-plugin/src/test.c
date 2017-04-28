#include <stdio.h>
#include "speck.h"
#include <string.h>
int main () {
    KEYTYPE initKey[] = {0x12457863, 0x73194682, 0x97436155};
    // int i = 0;
    // for (; i < 1000; i++) {

    // }
    KEYTYPE keys[26] = {0};
    // key_schedule(initKey, keys);
    int i;
    for (i = 0; i < 1000; i++) {
        memset(keys, 0, sizeof(keys));
        key_schedule(initKey, keys);
        // printf("0x%08x, ", keys[i]);
    }
    printf("\n");
}