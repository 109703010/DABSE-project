#ifndef TEST_H
#define TEST_H
#include "setup.h"
#include "enc.h"
#include "policy/getOmega.h"
#include "trapgen.h"
#include "attribute/Attribute.h"

typedef struct {
  int VALID; // 0 implies perp
  char* K;
  element_t W;
  element_t Z;
  unsigned char* V;
} INTER_CIPHER;

INTER_CIPHER C_prime;

void test(PUB_INFO epsilon, CIPHERTEXT C, TRAPDOOR ts) {
  C_prime.VALID = 0;
  char* IS = AttributeNameConversion(ts.IS, ts.ISSize);
  int* A_prime[C.A.l];
  int l_prime = 0;
  int row2orderTable[C.A.l];
  for(int i = 0; i < C.A.l; ++i) {
	for(int j = 0; j < ts.ISSize; ++j) {
	  if(IS[j] == C.A.rho[i]) {
		A_prime[l_prime] = C.A.M[i];
		row2orderTable[l_prime++] = j;
		break;
	  }
	}
  }
  free(IS);
  int* row = get_indepedent_row(A_prime, l_prime, C.A.n);
  int* omega = get_omega(A_prime, row, C.A.n);
  if(!isValidOmega(omega)) {
	return;
  }
  element_t* Kj1[2] = {&ts.K01, &ts.K11};
  element_t* Kj2[2] = {&ts.K02, &ts.K12};
  element_t* Kj3[2] = {ts.K03, ts.K13};
  element_t* Zj[2] = {&C.Z0, &C.Z1};
  element_t* Cj[2] = {&C.C0, &C.C1};
  mpz_t omega_i;
  element_t X_omega, pi_X_omega, e_X_omega_K;
  element_t K_omega, pi_K_omega, Kj1_inverse, e_Z_K_K;
  element_t Ej;
  mpz_init(omega_i);
  element_init_G1(X_omega, epsilon.pairing_of_G);
  element_init_G1(pi_X_omega, epsilon.pairing_of_G);
  element_init_GT(e_X_omega_K, epsilon.pairing_of_G);
  element_init_G1(K_omega, epsilon.pairing_of_G);
  element_init_G1(pi_K_omega, epsilon.pairing_of_G);
  element_init_G1(Kj1_inverse, epsilon.pairing_of_G);
  element_init_GT(e_Z_K_K, epsilon.pairing_of_G);
  element_init_GT(Ej, epsilon.pairing_of_G);
  int b;
  for(int j = 0; j < 2; ++j) {
	element_init_GT(Ej, epsilon.pairing_of_G);
	element_set1(pi_X_omega);
	element_set1(pi_K_omega);
	for(int i = 1; i <= row[0]; ++i) {
	  mpz_set_si(omega_i, omega[row[i]]);
	  element_pow_mpz(X_omega, C.X[row[i]], omega_i);
	  element_pow_mpz(K_omega, Kj3[j][row2orderTable[row[i]]], omega_i);
	  element_mul(pi_X_omega, pi_X_omega, X_omega);
	  element_mul(pi_K_omega, pi_K_omega, K_omega);
	}
	element_pairing(e_X_omega_K, pi_X_omega, *Kj2[j]);
	element_invert(Kj1_inverse, *Kj1[j]);
	element_mul(pi_K_omega, pi_K_omega, Kj1_inverse);
	element_pairing(e_Z_K_K, *Zj[j], pi_K_omega);
	element_mul(Ej, *Cj[j], e_X_omega_K);
	element_mul(Ej, Ej, e_Z_K_K);
	size_t len = element_length_in_bytes(Ej);
	char* Ej_Str = (char*)malloc(sizeof(char) * len);
	element_to_bytes(Ej_Str, Ej);
	unsigned char* tmpStr;
	epsilon.H(&tmpStr, Ej_Str, len, mpz_get_ui(epsilon.lambda));
	if(memcmp(tmpStr, C.V1, mpz_get_ui(epsilon.lambda)) == 0) {
	  C_prime.VALID = 1;
	  b = j;
	  break;
	}
  }
  element_clear(e_Z_K_K);
  element_clear(Kj1_inverse);
  element_clear(pi_K_omega);
  element_clear(K_omega);
  element_clear(e_X_omega_K);
  element_clear(pi_X_omega);
  element_clear(X_omega);
  if(!C_prime.VALID) {
	element_clear(Ej);
	return;
  }

  gmp_randstate_t state;
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));
  mpz_t s_prime;
  mpz_init(s_prime);
  mpz_urandomm(s_prime, state, epsilon.N);
  gmp_randclear(state);

  C_prime.K = strdup(C.K);
  
  element_t* Kb4[2] = {&ts.K14, &ts.K04};
  element_t* Kb5[2] = {&ts.K15, &ts.K05};
  element_t e_g_g_beta_mu_s_prime;
  element_init_GT(C_prime.W, epsilon.pairing_of_G);
  element_init_GT(e_g_g_beta_mu_s_prime, epsilon.pairing_of_G);
  element_pairing(e_g_g_beta_mu_s_prime, *Kb4[b], *Kb5[b]);
  element_pow_mpz(e_g_g_beta_mu_s_prime, e_g_g_beta_mu_s_prime, s_prime);
  element_mul(C_prime.W, Ej, e_g_g_beta_mu_s_prime);
  element_clear(e_g_g_beta_mu_s_prime);
  element_clear(Ej);

  element_t* gb[2] = {&epsilon.g3, &epsilon.g2};
  element_init_G1(C_prime.Z, epsilon.pairing_of_G);
  element_pow_mpz(C_prime.Z, *gb[b], s_prime);
  element_mul(C_prime.Z, C_prime.Z, *Zj[b]);
  mpz_clear(s_prime);
  
  C_prime.V = (unsigned char*)malloc(sizeof(unsigned char) * mpz_get_ui(epsilon.gamma));
  memcpy(C_prime.V, C.V2, mpz_get_ui(epsilon.gamma));
}
#endif
