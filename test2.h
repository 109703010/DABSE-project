#ifndef TEST2_H
#define TEST2_H
#include "setup.h"
#include "test.h"
#include "trapgen.h"

element_t symmetric_key;

int test2() {
    element_t v;
    mpz_t pow;
    unsigned char *v_str, *check;
    mpz_init_set_si(pow, -1);
    element_init_GT(symmetric_key, epsilon.pairing_of_G);
    element_init_G1(v, epsilon.pairing_of_G);
    element_pairing(v, C_prime.Z, delta.U);
    element_pow_mpz(v, v, pow);
    element_mul(symmetric_key, v, C_prime.W);
	int n = element_length_in_bytes(symmetric_key);
    v_str = pbc_malloc(n);
	element_to_bytes(v_str, symmetric_key);
    epsilon.H(&check, v_str, strlen(v_str), mpz_get_ui(epsilon.lambda));
	pbc_free(v_str);
    if (!strcmp(check, C_prime.V)) {
        return 1;
    } else {
        return 0;
    }
}

#endif
