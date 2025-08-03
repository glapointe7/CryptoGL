#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Serpent.h" // Doit correspondre au header de serpent.c

extern void setkey(void *ctx, const u8 *key, unsigned int keylen);
extern void encrypt(void *ctx, u8 *dst, const u8 *src);
extern void decrypt(void *ctx, u8 *dst, const u8 *src);

void hexToBytes(const char *hex, uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        sscanf(hex + 2*i, "%2hhx", &bytes[i]);
    }
}

void printHex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i)
        printf("%02X", data[i]);
    printf("\n");
}

int main() {
    struct serpent_ctx *ctx;
    uint8_t key_bytes[16];
    uint8_t pt_bytes[16];
    uint8_t pt[16], ct[16], decrypted[16];

    hexToBytes("2BD6459F82C5B300952C49104881FF48", key_bytes, 16);
    hexToBytes("EA024714AD5C4D84EA024714AD5C4D84", pt_bytes, 16);

    memcpy(pt, pt_bytes, 16);
    setkey(ctx, key_bytes, 16);
    encrypt(ctx, pt, ct);
    decrypt(ctx, ct, pt);

    printf("Plaintext:  ");
    printHex((uint8_t*)pt, 16);
    printf("Ciphertext: ");
    printHex((uint8_t*)ct, 16);
    printf("Decrypted:  ");
    printHex((uint8_t*)decrypted, 16);
    return 0;
}