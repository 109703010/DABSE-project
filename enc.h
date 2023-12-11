#ifndef ENC_H
#define ENC_H
#include "setup.h"
#include "policy/createPolicy.h"
#include "ciphertext/SymmetricCipher.h"

mpz_t backup_s;
//#define DEBUG = 1

typedef struct ciphertext {
  char* K;
  element_t C0;
  element_t C1;
  element_t Z0;
  element_t Z1;
  element_t* X;
  unsigned char* V1;
  unsigned char* V2;
  Policy A;
} CIPHERTEXT;

CIPHERTEXT C;

void enc(PUB_INFO epsilon, char* message, Policy A, char** T);

#if defined(DEBUG)
int main(void) {
  mpz_t lambda;
  mpz_init_set_ui(lambda, 256);
  setup(lambda);
  char* message = "This project almost killed us. : ( \n I would appreciate it if my professor gives me a 100.";
  char* accessString = "((A & D) | ((A & B) & C))"; 
  Policy A = convertToPolicy(accessString);
  char* T[] = {"Central Hospital", "311-4321", "City Hospital", "Genetics", "Male"};
  enc(epsilon, message, A, T);
  return 0;
}
#endif

void helper_element_pow_mpz(element_t x, element_t a, mpz_t n){
  if(mpz_cmp_si(n, 0) >= 0){
	element_pow_mpz(x, a, n);
  } else {
	mpz_t n_inverse;
	mpz_init(n_inverse);
	mpz_mul_si(n_inverse, n, -1);
	element_pow_mpz(x, a, n_inverse);
	element_invert(x, x);
  }
}

void enc(PUB_INFO epsilon, char* message, Policy A, char** T) {
  element_t k;
  element_init_GT(k, epsilon.pairing_of_G);
  element_random(k);

  mpz_t s;
  mpz_t* v = (mpz_t*)malloc(sizeof(mpz_t) * A.n);
  mpz_t* lambda = (mpz_t*)malloc(sizeof(mpz_t) * A.l);

  gmp_randstate_t state;
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));

  for(int i = 0; i < A.n; ++i) {
	mpz_init(v[i]);
	mpz_urandomm(v[i], state, epsilon.N);
  }
  gmp_randclear(state);
  mpz_init_set(s, v[0]);
  mpz_t product;
  mpz_init(product);
  for(int i = 0; i < A.l; ++i) {
	mpz_init(lambda[i]);
	for(int j = 0; j < A.n; ++j) {
	  mpz_mul_si(product, v[j], A.M[i][j]);
	  mpz_add(lambda[i], lambda[i], product);
	}
  }
  mpz_clear(product);
  for(int i = 0; i < A.n; ++i) {
	mpz_clear(v[i]);
  }
  free(v);


  C.K = (char*)malloc(sizeof(char) * (strlen(message) + EVP_MAX_BLOCK_LENGTH));
  int len = SEnc(message, strlen(message), k, C.K);
  C.K[len] = '\0';
  element_init_GT(C.C0, epsilon.pairing_of_G);
  element_mul(C.C0, epsilon.e_g1_g1_alpha, epsilon.e_g2_g2_alpha);
  element_pow_mpz(C.C0, C.C0, s);
  element_mul(C.C0, C.C0, k);

  element_init_GT(C.C1, epsilon.pairing_of_G);
  element_mul(C.C1, epsilon.e_g1_g1_alpha, epsilon.e_g3_g3_alpha);
  element_pow_mpz(C.C1, C.C1, s);
  element_mul(C.C1, C.C1, k);

  element_init_G1(C.Z0, epsilon.pairing_of_G);
  element_mul(C.Z0, epsilon.g1, epsilon.g2);
  element_pow_mpz(C.Z0, C.Z0, s);
  
  element_init_G1(C.Z1, epsilon.pairing_of_G);
  element_mul(C.Z1, epsilon.g1, epsilon.g3);
  element_pow_mpz(C.Z1, C.Z1, s);

  C.X = (element_t*)malloc(sizeof(element_t) * A.l);
  element_t g2_g3_a, g2_g3_a_lambda;
  element_init_G1(g2_g3_a, epsilon.pairing_of_G);
  element_init_G1(g2_g3_a_lambda, epsilon.pairing_of_G);
  element_mul(g2_g3_a, epsilon.g2_a, epsilon.g3_a);
  element_t H2_result, H3_result, H2_H3_result;
  element_init_G1(H2_result, epsilon.pairing_of_G);
  element_init_G1(H3_result, epsilon.pairing_of_G);
  element_init_G1(H2_H3_result, epsilon.pairing_of_G);
  for(int i = 0; i < A.l; ++i) {
	element_init_G1(C.X[i], epsilon.pairing_of_G);

	helper_element_pow_mpz(g2_g3_a_lambda, g2_g3_a, lambda[i]);

	epsilon.H_2(H2_result, T[i], strlen(T[i]), epsilon.g1, epsilon.g2);
	epsilon.H_2(H3_result, T[i], strlen(T[i]), epsilon.g1, epsilon.g3);
	element_mul(H2_H3_result, H2_result, H3_result);
	element_pow_mpz(H2_H3_result, H2_H3_result, s);
	element_invert(H2_H3_result, H2_H3_result);

	element_mul(C.X[i], g2_g3_a_lambda, H2_H3_result);
  }

  element_clear(H2_H3_result);
  element_clear(H3_result);
  element_clear(H2_result);
  element_clear(g2_g3_a_lambda);

  char* tmpStr;
  element_t k_e_g1_g1_alpha_s;
  element_init_GT(k_e_g1_g1_alpha_s, epsilon.pairing_of_G);
  element_pow_mpz(k_e_g1_g1_alpha_s, epsilon.e_g1_g1_alpha, s);
  element_mul(k_e_g1_g1_alpha_s, k_e_g1_g1_alpha_s, k);
  size_t n = element_length_in_bytes(k_e_g1_g1_alpha_s);
  tmpStr = (char*)malloc(sizeof(char) * n);
  element_to_bytes(tmpStr, k_e_g1_g1_alpha_s);
  tmpStr[n-1] = '\0';
  epsilon.H(&(C.V1), tmpStr, n, mpz_get_ui(epsilon.gamma));
  free(tmpStr);

  n = element_length_in_bytes(k);
  tmpStr = (char*)malloc(sizeof(char) * n);
  element_to_bytes(tmpStr, k);
  tmpStr[n-1] = '\0';
  epsilon.H(&(C.V2), tmpStr, strlen(tmpStr), mpz_get_ui(epsilon.lambda));
  free(tmpStr);

  C.A = A;

  for(int i = 0; i < A.l; ++i) {
	mpz_clear(lambda[i]);
  }
  free(lambda);
  mpz_init_set(backup_s, s);
  mpz_clear(s);
  element_clear(k);
}
#endif
