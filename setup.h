#include <gmp.h>
#include <openssl/sha.h>
#include <pbc/pbc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUG = 1;

typedef struct pub_info {
    pairing_t pairing_of_G;
    mpz_t N;
    element_t g1, g2, g3, g2_a, g3_a;
    element_t e_g1_g1_alpha, e_g2_g2_alpha, e_g3_g3_alpha;
    void (*H)(unsigned char**, char*, int, int);
    void (*H_2)(element_t, char*, int, element_t, element_t);
} PUB_INFO;

PUB_INFO epsilon;

typedef struct secret {
    element_t g1_alpha;
    // add secret key alpha
    mpz_t alpha;
    void (*H_2_)(element_t, char*, int, element_t);
} SECRET;

SECRET S;

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

/// @brief {0,1}∗ → G_p2
/// @param hash: output
/// @param input_str: array of char
/// @param input_size: count by byte
/// @param g: generator of G_p2
void secret_H_2_(element_t hash, char* input_str, int input_size, element_t g) {
    element_init_same_as(hash, g);
    element_from_hash(hash, input_str, input_size);
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
    mpz_init(exp);
    mpz_init_set_ui(exp, 59305);
    element_init_same_as(R, g_1);
    secret_H_2_(hash, input_str, input_size, g_2);
    element_pow_mpz(R, g_1, exp);
    element_mul(hash, R, hash);
}

/// @brief pub info: epsilon and secret: S are global variables
/// @param lambda
void setup(mpz_t lambda) {
    ORDER group_order;
    GENERATOR gen;
    pbc_param_t custom_param;
    element_t check;
    mpz_t tmp, gamma, alpha, a;
    gmp_randstate_t state;

    mpz_init(group_order.N);
    mpz_init_set_ui(group_order.p_1, 941083987);
    mpz_init_set_ui(group_order.p_2, 573259433);
    mpz_init_set_ui(group_order.p_3, 334214467);
    mpz_mul(group_order.N, group_order.p_1, group_order.p_2);
    mpz_mul(group_order.N, group_order.N, group_order.p_3);
    mpz_init_set(epsilon.N, group_order.N);
#if defined(DEBUG)
    gmp_printf("p1: %Zd \n", group_order.p_1);
    gmp_printf("p2: %Zd \n", group_order.p_2);
    gmp_printf("p3: %Zd \n", group_order.p_3);
    gmp_printf("N: %Zd \n", group_order.N);
#endif
    mpz_init(tmp);
    pbc_param_init_a1_gen(custom_param, group_order.N);  // 使用N作为order
    pairing_init_pbc_param(epsilon.pairing_of_G, custom_param);
    element_init_G1(gen.g, epsilon.pairing_of_G);    // 创建G的生成元
    element_init_G1(gen.g_1, epsilon.pairing_of_G);  // 创建G的生成元
    element_init_G1(gen.g_2, epsilon.pairing_of_G);  // 创建G的生成元
    element_init_G1(gen.g_3, epsilon.pairing_of_G);  // 创建G的生成元
    element_init_G1(check, epsilon.pairing_of_G);
    do {
        element_random(gen.g_1);  // 随机选择一个生成元
        mpz_mul(tmp, group_order.p_2, group_order.p_3);
        element_pow_mpz(check, gen.g_1, tmp);  // 计算 g^(N/p1)
    } while (element_is1(check));              // g^(N/p1)==identity
    do {
        element_random(gen.g_2);  // 随机选择一个生成元
        mpz_mul(tmp, group_order.p_1, group_order.p_3);
        element_pow_mpz(check, gen.g_2, tmp);  // 计算 g^(N/p2)
    } while (element_is1(check));              // g^(N/p2)==identity
    do {
        element_random(gen.g_3);  // 随机选择一个生成元
        mpz_mul(tmp, group_order.p_1, group_order.p_2);
        element_pow_mpz(check, gen.g_3, tmp);  // 计算 g^(N/p3)
    } while (element_is1(check));              // g^(N/p3)==identity
    element_init_same_as(epsilon.g1, gen.g_1);
    element_init_same_as(epsilon.g2, gen.g_2);
    element_init_same_as(epsilon.g3, gen.g_3);

    mpz_init(gamma);
    mpz_init(alpha);
    mpz_init(a);
    gmp_randinit_default(state);
    mpz_urandomm(gamma, state, lambda);
    mpz_urandomm(alpha, state, group_order.N);
    mpz_urandomm(a, state, group_order.N);
#if defined(DEBUG)
    gmp_printf("lambda: %Zd \n", lambda);
    gmp_printf("gamma: %Zd \n", gamma);
    gmp_printf("alpha: %Zd \n", alpha);
    gmp_printf("a: %Zd \n", a);
#endif
    element_init_GT(epsilon.e_g1_g1_alpha, epsilon.pairing_of_G);
    element_init_GT(epsilon.e_g2_g2_alpha, epsilon.pairing_of_G);
    element_init_GT(epsilon.e_g3_g3_alpha, epsilon.pairing_of_G);
    element_pairing(epsilon.e_g1_g1_alpha, gen.g_1, gen.g_1);              // r11 = e(g1, g1)
    element_pairing(epsilon.e_g2_g2_alpha, gen.g_2, gen.g_2);              // r22 = e(g2, g2)
    element_pairing(epsilon.e_g3_g3_alpha, gen.g_3, gen.g_3);              // r11 = e(g1, g1)
    element_mul_mpz(epsilon.e_g1_g1_alpha, epsilon.e_g1_g1_alpha, alpha);  // r11 = r11^alpha
    element_mul_mpz(epsilon.e_g2_g2_alpha, epsilon.e_g2_g2_alpha, alpha);  // r22 = r22^alpha
    element_mul_mpz(epsilon.e_g3_g3_alpha, epsilon.e_g3_g3_alpha, alpha);  // r33 = r33^alpha
    element_init_same_as(S.g1_alpha, gen.g_1);
    element_init_same_as(epsilon.g2_a, gen.g_2);
    element_init_same_as(epsilon.g3_a, gen.g_3);
    element_mul_mpz(S.g1_alpha, S.g1_alpha, alpha);
    element_mul_mpz(epsilon.g2_a, epsilon.g2_a, a);
    element_mul_mpz(epsilon.g3_a, epsilon.g3_a, a);
    // add secret key alpha
    mpz_init(S.alpha);
    mpz_init_set(S.alpha, alpha);
#if defined(DEBUG)
    char str[] = "hello world";
    int size = 12;
    unsigned char* hash;
    epsilon.pub_info_H(&hash, str, size, mpz_get_ui(gamma));
    // 打印哈希值的十六进制表示
    for (int i = 0; i < mpz_get_ui(gamma); i++) {
        printf("%02x", *(hash + i));
    }
    printf("\n");
    element_t hash_g;
    S.secret_H_2_(hash_g, str, size, gen.g_2);
    epsilon.pub_info_H_2(hash_g, str, size, gen.g_1, gen.g_2);
#endif
}

#if defined(DEBUG)
int main() {
    mpz_t lambda;
    mpz_init_set_ui(lambda, 256);
    setup(lambda);
    return 0;
}
#endif
