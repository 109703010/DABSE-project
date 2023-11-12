#ifndef DEC_H
#define DEC_H
#include "setup.h"
#include "test2.h"
#include "ciphertext/SymmetricCipher.h"

void Dec(PUB_INFO, SECRET, INTER_CIPHER);

void Dec(PUB_INFO epsilon, SECRET S, INTER_CIPHER C_prime){
    int len = SDec(message, strlen(message), k, C_prime.K);
};

#if defined(DEBUG)
int main(void){
    mpz_t lambda;
    mpz_init_set_ui(lambda, 256);
    setup(lambda);
    char* message = "This project almost killed us. : ( \n I would appreciate it if my professor gives me a 100.";
    char* accessString = "((A & D) | ((A & B) & C))"; 
    Policy A = convertToPolicy(accessString);
    char* T[] = {"Central Hospital", "311-4321", "City Hospital", "Genetics", "Male"};
    Enc(epsilon, message, A, T);
    char* InputValueSet[ATTRIBUTESETSIZE];
	InputValueSet[0] = "City";
	InputValueSet[1] = "Cardiologist";
	InputValueSet[2] = "Male";
	InputValueSet[3] = "105-2568";
    trapgen(epsilon, S, InputValueSet);
    // add test, test2
    Dec(epsilon, S, C_prime);
    return 0;
}
#endif
#endif