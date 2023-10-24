#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <pbc/pbc.h>
#include <time.h>

// for attribute set index & value
int AttributeSetSize = 10;

// don't know useful or not
// // Define a big integer for n
// mpz_t n;
// mpz_init(n);
// mpz_set_str(n, "1234567890", 10);

typedef struct PK{
	pairing_t pairing;
	element_t g;
	element_t N;
	element_t alpha;
	element_t g1;
	element_t g2;
	element_t g3;
	element_t g2a;
	element_t g3a;
	element_t eg1alpha;
	element_t eg2alpha;
	element_t eg3alpha;			
}PK;

void hash(const unsigned char* input, size_t input_len, unsigned char* output, size_t a, unsigned int N)
void hash1(const unsigned char* input, size_t input_len, unsigned char* output, size_t a, unsigned int N)
void hash2(const unsigned char* input, size_t input_len, unsigned char* output, size_t a, unsigned int N)
void hash3(const unsigned char* input, size_t input_len, unsigned char* output, size_t a, unsigned int N)

typedef struct SK{
	element_t g1alpha;
	element_t (*hash2pl)(char* x)
	element_t (*hash3pl)(char* y)
}SK;

void hash2pl(const unsigned char* input, size_t input_len, unsigned char* output, size_t a, unsigned int N)
void hash3pl(const unsigned char* input, size_t input_len, unsigned char* output, size_t a, unsigned int N)

typedef struct Ts{
	element_t K01;
	element_t K02;
	element_t K11;
	element_t K12;
	element_t K03[AttributeSetSize];
	element_t K13[AttributeSetSize];
	element_t K04;
	element_t K05;
	element_t K14;
	element_t K15;
}Ts;

// AS need to do some research
typedef struct AS{
	char *LS[AttributeSetSize];
}AS;

typedef struct Delta{
	int b;
	element_t U;
}Delta;

void trapgen(Pk* pk, Sk* sk, AS* S, Ts* ts, Delta* delta){
	// setting
	srand(time(NULL));
	delta.b = rand() % 2;
	element_t t, u, beta;
	element_init_Zr(t, pairing);
	element_init_Zr(u, pairing);
	element_init_Zr(beta, pairing);
	element_random(t);
	element_random(u);
	element_random(beta);
	element_mod(t, t, pk.N);
	element_mod(u, u, pk.N);
	element_mod(beta, beta, pk.N);
	// L & L' don't know how to do
	AS Spl;
	for(int i=0; i<AttributeSetSize; ++i){
		Spl.LS[i] = "compare";
	}

	// for calculate
	element_t g2alpha, g3alpha;
	element_init_G1(g2alpha, pk.pairing);
	element_init_G1(g3alpha, pk.pairing);
	element_pow_zn(g2alpha, pk.g2, pk.alpha);
	element_pow_zn(g3alpha, pk.g3, pk.alpha);

	element_init_G1(ts.K01, pk.pairing);
	element_pow_zn(ts.K01, pk.g2a, t);
	element_mul(ts.K01, ts.K01, g2alpha);	// K0,1
    element_init_G1(ts.K02, pk.pairing);
	element_pow_zn(ts.K02, pk.g2, t)	// K0,2
    element_init_G1(ts.K11, pk.pairing);
	element_pow_zn(ts.K11, pk.g3a, t);
	element_mul(ts.K11, ts.K11, g3alpha);	// K1,1
    element_init_G1(ts.K12, pk.pairing);
	element_pow_zn(ts.K12, pk.g3, t)	// K1,2
    for(int i=0; i<AttributeSetSize; ++i){
		element_init_G1(ts.K03, pk.pairing);
	    element_init_G1(ts.K13, pk.pairing);
		if(delta.b = 0){
			element_from_hash(ts.K03, sk.hash2pl(S.LS[i]), SHA384_DIGEST_LENGTH);
			element_pow_zn(ts.K03, ts.K03, t);	// K0,3,i
			element_from_hash(ts.K13, sk.hash3pl(Spl.LS[i]), SHA384_DIGEST_LENGTH);
			element_pow_zn(ts.K13, ts.K13, t);	// K1,3,i
		}else{
			element_from_hash(ts.K03, sk.hash3pl(S.LS[i]), SHA384_DIGEST_LENGTH);
			element_pow_zn(ts.K03, ts.K03, t);	// K0,3,i
			element_from_hash(ts.K13, sk.hash2pl(Spl.LS[i]), SHA384_DIGEST_LENGTH);
			element_pow_zn(ts.K13, ts.K13, t);	// K1,3,i
		}
	}
    element_init_G1(ts.K04, pk.pairing);
	element_pow_zn(ts.K04, pk.g2, beta);	// K0,4
    element_init_G1(ts.K05, pk.pairing);
	element_pow_zn(ts.K05, pk.g2, u);	// K0,5
    element_init_G1(ts.K14, pk.pairing);
	element_pow_zn(ts.K14, pk.g3, beta);	// K1,4
    element_init_G1(ts.K15, pk.pairing);
	element_pow_zn(ts.K15, pk.g3, u);	// K1,5

	// delta U
	element_init_G1(delta.U, pairing);
	if(delta.b == 0){
		element_pow_zn(delta.U, pk.g3, beta);
		element_pow_zn(delta.U, delta.U, u);
	}else{
		element_pow_zn(delta.U, pk.g2, beta);
		element_pow_zn(delta.U, delta.U, u);
	}
	element_mul(delta.U, delta.U, sk.g1alpha);

	// clear element
	element_clear(t);
	element_clear(u);
	element_clear(beta);
	element_clear(g2alpha);
	element_clear(g3alpha);
}

int main() {
	PK pk;
	SK sk;
	AS S;
	Ts ts;
	Delta delta;
	for(int i=0; i<AttributeSetSize; ++i0){
		S.LS[i] = "test";
	}
	trapgen(pk, sk, S);
}