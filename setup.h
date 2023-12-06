#ifndef SETUP_H
#define SETUP_H
#include <gmp.h>
#include <openssl/sha.h>
#include <pbc/pbc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUG = 1;

typedef struct pub_info {
    pairing_t pairing_of_G;
    mpz_t N, gamma, lambda;
    element_t g1, g2, g3, g2_a, g3_a;
    element_t e_g1_g1_alpha, e_g2_g2_alpha, e_g3_g3_alpha;
    void (*H)(unsigned char**, char*, int, int);
    void (*H_2)(element_t, char*, int, element_t, element_t);
} PUB_INFO;

typedef struct secret {
    element_t g1_alpha;
    // add secret key alpha
    mpz_t alpha;
    void (*H_2_)(element_t, char*, int, element_t);
} SECRET;

PUB_INFO epsilon;
SECRET S;

// get rid of warning messages
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

/// @brief: {0, 1}^* -> {0, 1}^(gamma)
/// @param hash: output
/// @param input_string: array of char
/// @param input_size: count by byte
/// @param output_size: count by byte
void pub_info_H(unsigned char** hash, char* input_str, int input_size, int output_size) {
    *hash = (unsigned char*)malloc((output_size) * sizeof(unsigned char));
    unsigned char sha256_hash[SHA256_DIGEST_LENGTH];
    // 创建SHA-256哈希对象
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    // 更新哈希对象以处理输入字符串
    SHA256_Update(&sha256, input_str, input_size);
    // 计算SHA-256哈希值
    SHA256_Final(sha256_hash, &sha256);
    // 截断SHA-256哈希值到所需长度
    memcpy(*hash, sha256_hash, output_size);
    // 打印哈希值的十六进制表示
}

#pragma GCC diagnostic pop

/// @brief {0,1}∗ → G_p2
/// @param hash: output
/// @param input_str: array of char
/// @param input_size: count by byte
/// @param g: generator of G_p2
void secret_H_2_(element_t hash, char* input_str, int input_size, element_t g) {
    mpz_t exp;
    mpz_init_set_ui(exp, 59305);
    element_init_same_as(hash, g);
    element_pow_mpz(hash, g, exp);
    element_from_hash(hash, input_str, input_size);
    mpz_clear(exp);
}

/// @brief {0,1}∗ → G_p1p2
/// @param hash: output
/// @param input_str: array of char
/// @param input_size: count by byte
/// @param g_1: generator of G_p1
/// @param g_2: generator of G_p2
void pub_info_H_2(element_t hash, char* input_str, int input_size, element_t g_1, element_t g_2) {
    mpz_t exp;
    element_t R;
    mpz_init_set_ui(exp, 59305);
    element_init_same_as(R, g_1);
    element_pow_mpz(R, g_1, exp);
    secret_H_2_(hash, input_str, input_size, g_2);
    element_mul(hash, R, hash);
    mpz_clear(exp);
    // element_free(R);
}

/// @brief pub info: epsilon and secret: S are global variables
/// @param lambda
void setup(mpz_t lambda) {
    pbc_param_t custom_param;
    mpz_t a, p1, p2, p3, tmp;
    gmp_randstate_t state;
    element_t check;

    mpz_init(epsilon.N);
    mpz_init(tmp);
    mpz_init_set_ui(p1, 941083987);
    mpz_init_set_ui(p2, 573259433);
    mpz_init_set_ui(p3, 334214467);
    // mpz_init_set_ui(p1, 139);
    // mpz_init_set_ui(p2, 139);
    // mpz_init_set_ui(p3, 139);
    mpz_mul(epsilon.N, p1, p2);
    mpz_mul(epsilon.N, epsilon.N, p3);

    mpz_init(epsilon.gamma);
    mpz_init_set(epsilon.lambda, lambda);
    mpz_init(S.alpha);
    mpz_init(a);
    gmp_randinit_default(state);
    mpz_urandomm(epsilon.gamma, state, lambda);
    mpz_urandomm(S.alpha, state, epsilon.N);
    mpz_urandomm(a, state, epsilon.N);
#if defined(DEBUG)
    gmp_printf("p1: %Zd \n", p1);
    gmp_printf("p2: %Zd \n", p2);
    gmp_printf("p3: %Zd \n", p3);
    gmp_printf("N: %Zd \n", epsilon.N);
    gmp_printf("alpha: %Zd \n", S.alpha);
    gmp_printf("a: %Zd \n", a);
    gmp_printf("gamma: %Zd \n\n", epsilon.gamma);
#endif
    pbc_param_init_a1_gen(custom_param, epsilon.N);
    pairing_init_pbc_param(epsilon.pairing_of_G, custom_param);
    element_init_G1(epsilon.g1, epsilon.pairing_of_G);
    element_init_G1(epsilon.g2, epsilon.pairing_of_G);
    element_init_G1(epsilon.g3, epsilon.pairing_of_G);
    element_init_G1(check, epsilon.pairing_of_G);

    do {
        element_random(epsilon.g1);  // 随机选择一个生成元
        mpz_mul(tmp, p2, p3);
        element_pow_mpz(epsilon.g1, epsilon.g1, tmp);  // 计算 g^(N/p1)
    } while (element_is0(epsilon.g1));                 // g^(N/p1)==identity
    do {
        element_random(epsilon.g2);  // 随机选择一个生成元
        mpz_mul(tmp, p1, p3);
        element_pow_mpz(epsilon.g2, epsilon.g2, tmp);  // 计算 g^(N/p2)
    } while (element_is0(epsilon.g2));                 // g^(N/p2)==identity
    do {
        element_random(epsilon.g3);  // 随机选择一个生成元
        mpz_mul(tmp, p1, p2);
        element_pow_mpz(epsilon.g3, epsilon.g3, tmp);  // 计算 g^(N/p3)
    } while (element_is0(epsilon.g3));                 // g^(N/p3)==identity
#if defined(DEBUG)
    mpz_add_ui(tmp, p1, 1);
    element_pow_mpz(check, epsilon.g1, tmp);
    element_printf("g1: %B\n", epsilon.g1);
    element_printf("g1^(p1+1): %B\n\n", check);
    mpz_add_ui(tmp, p2, 1);
    element_pow_mpz(check, epsilon.g2, tmp);
    element_printf("g2: %B\n", epsilon.g2);
    element_printf("g2^(p2+1): %B\n\n", check);
    mpz_add_ui(tmp, p3, 1);
    element_pow_mpz(check, epsilon.g3, tmp);
    element_printf("g3: %B\n", epsilon.g3);
    element_printf("g3^(p3+1): %B\n\n", check);
    mpz_clear(tmp);
#endif
    element_init_GT(epsilon.e_g1_g1_alpha, epsilon.pairing_of_G);
    element_init_GT(epsilon.e_g2_g2_alpha, epsilon.pairing_of_G);
    element_init_GT(epsilon.e_g3_g3_alpha, epsilon.pairing_of_G);
    element_init_G1(S.g1_alpha, epsilon.pairing_of_G);
    element_init_G1(epsilon.g2_a, epsilon.pairing_of_G);
    element_init_G1(epsilon.g3_a, epsilon.pairing_of_G);

    element_pairing(epsilon.e_g1_g1_alpha, epsilon.g1, epsilon.g1);          // r11 = e(g1, g1)
    element_pairing(epsilon.e_g2_g2_alpha, epsilon.g2, epsilon.g2);          // r22 = e(g2, g2)
    element_pairing(epsilon.e_g3_g3_alpha, epsilon.g3, epsilon.g3);          // r11 = e(g1, g1)
    element_pow_mpz(epsilon.e_g1_g1_alpha, epsilon.e_g1_g1_alpha, S.alpha);  // r11 = r11^alpha
    element_pow_mpz(epsilon.e_g2_g2_alpha, epsilon.e_g2_g2_alpha, S.alpha);  // r22 = r22^alpha
    element_pow_mpz(epsilon.e_g3_g3_alpha, epsilon.e_g3_g3_alpha, S.alpha);  // r33 = r33^alpha
    element_pow_mpz(S.g1_alpha, epsilon.g1, S.alpha);
    element_pow_mpz(epsilon.g2_a, epsilon.g2, a);
    element_pow_mpz(epsilon.g3_a, epsilon.g3, a);
#if defined(DEBUG)
    element_printf("e(g1, g1)^alpha: %B\n", epsilon.e_g1_g1_alpha);
    element_printf("e(g2, g2)^alpha: %B\n", epsilon.e_g2_g2_alpha);
    element_printf("e(g3, g3)^alpha: %B\n", epsilon.e_g3_g3_alpha);
    element_printf("g1^alpha: %B\n", S.g1_alpha);
    element_printf("g2^a: %B\n", epsilon.g2_a);
    element_printf("g3^a: %B\n\n", epsilon.g3_a);
#endif
    // fill the function ptr
    epsilon.H = pub_info_H;
    epsilon.H_2 = pub_info_H_2;
    S.H_2_ = secret_H_2_;
#if defined(DEBUG)
    char str[] = "hello world";
    unsigned char* hash;
    element_t hash_g;
    epsilon.H(&hash, str, strlen(str), mpz_get_ui(epsilon.gamma));
    // 打印哈希值的十六进制表示
    printf("H: ");
    for (int i = 0; i < mpz_get_ui(epsilon.gamma); i++) {
        printf("%02x", *(hash + i));
    }
    printf("\n");
    S.H_2_(hash_g, str, strlen(str), epsilon.g2);
    element_printf("H2_: %B\n", hash_g);
    epsilon.H_2(hash_g, str, strlen(str), epsilon.g1, epsilon.g2);
    element_printf("H2: %B\n", hash_g);
    free(hash);
    // element_free(hash_g);
#endif
    mpz_clear(a);
    mpz_clear(p1);
    mpz_clear(p2);
    mpz_clear(p3);
    // element_free(check);
}

#if defined(DEBUG)
int main() {
    mpz_t lambda;
    mpz_init_set_ui(lambda, 256);
    setup(lambda);
    mpz_clear(lambda);
    return 0;
}
#endif
#endif