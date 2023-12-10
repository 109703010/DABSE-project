#ifndef DEC_H
#define DEC_H
#include "setup.h"
#include "test2.h"
#include "ciphertext/SymmetricCipher.h"

void Dec(PUB_INFO, element_t, INTER_CIPHER, char*);

void Dec(PUB_INFO epsilon, element_t symmetric_key, INTER_CIPHER C_prime, char* plaintext){
    int len = SDec(C_prime.K, strlen(C_prime.K), symmetric_key, plaintext);
};

#endif
