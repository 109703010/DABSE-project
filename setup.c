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

typedef struct generator {
    element_t g;
    element_t g_1;
    element_t g_2;
    element_t g_3;
} GENERATOR;

void setup() {
    ORDER group_order;
    mpz_init_set_ui(group_order.p_1, 941083987);
    mpz_init_set_ui(group_order.p_2, 573259433);
    mpz_init_set_ui(group_order.p_3, 334214467);
    mpz_init(group_order.N);
    mpz_mul(group_order.N, group_order.p_1, group_order.p_2);
    mpz_mul(group_order.N, group_order.N, group_order.p_3);
#if defined(DEBUG)
    gmp_printf("p1: %Zd \n", group_order.p_1);
    gmp_printf("p2: %Zd \n", group_order.p_2);
    gmp_printf("p3: %Zd \n", group_order.p_3);
    gmp_printf("N: %Zd \n", group_order.N);
#endif
    pairing_t custom_pairing;
    pbc_param_t custom_param;
    GENERATOR gen;
    element_t check;
    mpz_t tmp;
    mpz_init(tmp);
    pbc_param_init_a1_gen(custom_param, group_order.N);  // 使用N作为order
    pairing_init_pbc_param(custom_pairing, custom_param);
    element_init_G1(gen.g, custom_pairing);    // 创建G的生成元
    element_init_G1(gen.g_1, custom_pairing);  // 创建G的生成元
    element_init_G1(gen.g_2, custom_pairing);  // 创建G的生成元
    element_init_G1(gen.g_3, custom_pairing);  // 创建G的生成元
    element_init_G1(check, custom_pairing);
    do {
        element_random(gen.g_1);  // 随机选择一个生成元
        mpz_mul(tmp, group_order.p_2, group_order.p_3);
        element_pow_mpz(check, gen.g_1, tmp);  // 计算 g^(N/p1)
    } while (element_is1(check));              // g^(N/p1)==identity
    do {
        element_random(gen.g_2);  // 随机选择一个生成元
        mpz_mul(tmp, group_order.p_1, group_order.p_3);
        element_pow_mpz(check, gen.g_2, tmp);  // 计算 g^(N/p2)
    } while (element_is1(check));              // g^(N/p1)==identity
    do {
        element_random(gen.g_3);  // 随机选择一个生成元
        mpz_mul(tmp, group_order.p_1, group_order.p_2);
        element_pow_mpz(check, gen.g_3, tmp);  // 计算 g^(N/p3)
    } while (element_is1(check));              // g^(N/p1)==identity
}

int main() {
    setup();
    return 0;
}