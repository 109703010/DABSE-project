#ifndef TEST2_H
#define TEST2_H
#include "setup.h"
#include "test.h"
#include "trapgen.h"

mpz_t symmetric_key;

int test2() {
    element_t v;
    mpz_t pow;
    unsigned char *v_str, *check;
    mpz_init_set_si(pow, -1);
    mpz_init(symmetric_key);
    element_init_G1(v, epsilon.pairing_of_G);
    element_pairing(v, C_prime.Z, delta.U);
    element_pow_mpz(v, v, pow);
    element_mul(v, v, C_prime.W);
    element_to_mpz(symmetric_key, v);
    v_str = mpz_get_str(NULL, 10, symmetric_key);
    epsilon.H(&check, v_str, strlen(v_str), mpz_get_ui(epsilon.lambda));
    if (!strcmp(check, C_prime.V)) {
        return 1;
    } else {
        return 0;
    }
}

#endif
