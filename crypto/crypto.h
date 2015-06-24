#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>

#ifndef TROPICANA_CRYPTO_H
#define TROPICANA_CRYPTO_H

#define MAX_INPUT_LEN 4096

typedef struct _Keyset {
	char pk[crypto_box_PUBLICKEYBYTES];
	char sk[crypto_box_SECRETKEYBYTES];
} Keyset;

typedef struct _self {
	unsigned char name[128];
	Keyset *keyset;
} Self;

typedef struct _Message {
	unsigned char *sender_id;
	unsigned char *receiver_id;
	unsigned char *plaintext;
	unsigned long long length;
	unsigned char nonce[crypto_box_NONCEBYTES];
} Message;

int crypto_init( void *data );

int crypto_gen_keyset( Keyset *result, void *data );
int crypto_gen_keyset2( Keyset *result, const unsigned char *seed, void *data );

int crypto_serialize( const Message *m, char *result, void *data );
int crypto_deserialize( const char *buf, Message *result, void *data );

#endif