#include <gmp.h>
#include <pbc/pbc.h>
#include <stdio.h>

#define DEBUG = 1;

typedef struct order {
    mpz_t p_1;
    mpz_t p_2;
    mpz_t p_3;
    mpz_t N;
} ORDER;

void setup() {
    ORDER group_order;
    mpz_init_set_ui(group_order.p_1, 941083987);
    mpz_init_set_ui(group_order.p_2, 573259433);
    mpz_init_set_ui(group_order.p_3, 334214467);
    mpz_mul(group_order.N, group_order.p_1, group_order.p_2);
    mpz_mul(group_order.N, group_order.N, group_order.p_3);
#if defined(DEBUG)
    gmp_printf("p1: %Zd \n", group_order.p_1);
    gmp_printf("p2: %Zd \n", group_order.p_2);
    gmp_printf("p3: %Zd \n", group_order.p_3);
    gmp_printf("N: %Zd \n", group_order.N);
#endif

    // pairing_t custom_pairing;
    // pbc_param_t custom_param;
    // element_t generator;

    // pbc_param_init_a1_gen(custom_param, N);  // 使用 N 作为阶
    // pairing_init_pbc_param(custom_pairing, custom_param);

    // element_init_G1(generator, custom_pairing);  // 创建 G1 群的生成元
}

int main() {
    setup();
    return 0;
}