#include <crypto.h>

static int print_hex( const void *bin, const size_t bin_len ) {
	char   *hex;
    size_t  hex_size;

    hex_size = bin_len * 2 + 1;
    if ((hex = malloc(hex_size)) == NULL) {
    	printf("%s", "malloc error");
    	exit(0);
    }

    /* the library supplies a few utility functions like the one below */
    if (sodium_bin2hex(hex, hex_size, bin, bin_len) == NULL) {
        printf("%s", "sodium_bin2hex failed.");
        exit(0);
    }
    printf("%s\n", hex);
    free(hex);
    return 0;
}

int crypto_init( void *data ) {
	sodium_init();
	return 0;
}

int crypto_serialize( const Message *m, char *result, void *data ) {
	return 0;
}

int crypto_deserialize( const char *buf, Message *result, void *data ) {
	return 0;
}

int crypto_gen_keyset( Keyset *result, void *data ) {
	if(result) {
		crypto_box_keypair(result->pk, result->sk);
		return 0;
	}
	return -1;
}

int crypto_gen_keyset2( Keyset *result, const unsigned char *seed, void *data ) {
	// HF: No implemention currently.
	return crypto_gen_keyset(result, data);
}

int main( int argc, char **argv ) {
	Self p1;
	Self p2;
	Keyset keys;
	Message message;
	char plaintext[MAX_INPUT_LEN] = "Huzefa Aziz Fatakdawala";
	char ciphertext[crypto_box_MACBYTES + MAX_INPUT_LEN];
	int nonce_index = 0;

	crypto_init(NULL);

	memcpy(p1.name, "Person 1", strlen("Person 1") + 1);
	memcpy(p2.name, "Person 2", strlen("Person 2") + 1);

	printf("Person 1 sends a message to Person 2.\n");
	printf("Generating keys....\n");
	crypto_gen_keyset( &keys, NULL );
	p1.keyset = malloc(sizeof(Keyset));
	memcpy(p1.keyset, &keys, sizeof(Keyset));

	crypto_gen_keyset( &keys, NULL );
	p2.keyset = malloc(sizeof(Keyset));
	memcpy(p2.keyset, &keys, sizeof(Keyset));

	// Generate Keys
	printf("Person 1 Public Key: ");
	print_hex(p1.keyset->pk, strlen(p1.keyset->pk));

	printf("Person 1 Secret Key: ");
	print_hex(p1.keyset->sk, strlen(p1.keyset->sk));

	printf("Person 2 Public Key: ");
	print_hex(p2.keyset->pk, strlen(p2.keyset->pk));

	printf("Person 2 Secret Key: ");
	print_hex(p2.keyset->sk, strlen(p2.keyset->sk));

	printf("\n%s", "Plaintext (4095 bytes)? ");
	fgets(plaintext, MAX_INPUT_LEN, stdin);

	//Person 1 will be sending a message to person 2.
	printf("\nBuilding message structure...\n");

	// HF: We're using names as ids for now. This will have to change.
	message.sender_id = p1.name;
	message.receiver_id = p2.name;
	message.plaintext = plaintext;
	message.length = strlen(plaintext) + 1;
	snprintf(message.nonce, crypto_box_NONCEBYTES, "%d", nonce_index); // sync messages

	printf("sender_id: %s\n", message.sender_id);
	printf("receiver_id: %s\n", message.receiver_id);
	printf("plaintext: %s\n", message.plaintext);
	printf("length: %llu\n", message.length);
	printf("Nonce: %s\n", message.nonce);

	// Encrypt
	printf("\nEncrypting...\n");
	crypto_box_easy(ciphertext, message.plaintext, message.length, message.nonce, p2.keyset->pk, p1.keyset->sk);
	printf("ciphertext: ");
	print_hex(ciphertext, crypto_box_MACBYTES + message.length);

	//Decrypt
	printf("\nDecrypting...\n");
	memset(plaintext, 0, MAX_INPUT_LEN);
	crypto_box_open_easy(plaintext, ciphertext, crypto_box_MACBYTES + message.length, message.nonce, p1.keyset->pk, p2.keyset->sk);
	printf("Return plaintext: ");
	fwrite(plaintext, 1U, message.length, stdout);
	printf("\n");

	return 0;
}