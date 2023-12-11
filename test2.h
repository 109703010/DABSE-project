#ifndef TEST2_H
#define TEST2_H
#include "setup.h"
#include "test.h"
#include "trapgen.h"

element_t symmetric_key;

int test2() {
    element_t v;
    unsigned char *v_str, *check;
    element_init_GT(symmetric_key, epsilon.pairing_of_G);
    element_init_GT(v, epsilon.pairing_of_G);
    element_pairing(v, C_prime.Z, delta.U);
	element_invert(v, v);
    element_mul(symmetric_key, v, C_prime.W);
	size_t n = element_length_in_bytes(symmetric_key);
    v_str = (char*)malloc(sizeof(char) * n);
	element_to_bytes(v_str, symmetric_key);
	v_str[n-1] = '\0';
    epsilon.H(&check, v_str, strlen(v_str), mpz_get_ui(epsilon.lambda));
	free(v_str);
    if (!memcmp(check, C_prime.V, mpz_get_ui(epsilon.lambda) / 8)) {
        return 1;
    } else {
        return 0;
    }
}

#endif
