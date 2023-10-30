#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <pbc/pbc.h>
#include <time.h>
#include "setup.h"

// for attribute set index & value
int AttributeSetSize = 4;

typedef struct AttributeSet {
    char* attribute_name;
    char** attribute_values;
    int num_values;
};

AttributeSet attributes[4]; // Assuming you have four attributes
// Hospital attribute
attributes[0].attribute_name = "Hospital";
attributes[0].attribute_values = (char*[]){"City", "People", "Central", "Provincial", ""};
attributes[0].num_values = 5;

// Department attribute
attributes[1].attribute_name = "Department";
attributes[1].attribute_values = (char*[]){"Cardiologist", "Stomatology", "Neurology", "Genetics", ""};
attributes[1].num_values = 5;

// Sex attribute
attributes[2].attribute_name = "Sex";
attributes[2].attribute_values = (char*[]){"Male", "Female", ""};
attributes[2].num_values = 3;

// Doctor number attribute
attributes[3].attribute_name = "Doctor number";
attributes[3].attribute_values = (char*[]){"105-2568", "021-6932", "231-6421", "311-4321", ""};
attributes[3].num_values = 5;

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
	int LS[AttributeSetSize];
}AS;

typedef struct Delta{
	int b;
	element_t U;
}Delta;

typedef struct Fakekey{
	element_t fakekey;
}Fakekey;

void trapgen(PUB_INFO* epsilon, S* S, AS* S_incline, Ts* ts, Delta* delta, Fakekey* fakekey){
	// Create, initialize, and seed a new random number generator.
	gmp_randstate_t state = new gmp_randstate_t();
	gmp_lib.gmp_randinit_mt(state);
	gmp_lib.gmp_randseed_ui(state, 100000U);
	// setting
	srand(time(NULL));
	delta.b = rand() % 2;
	mpz_t t, u, beta;
	mpz_init(t);
	mpz_init(u);
	mpz_init(beta);
	mpz_urandomm(t, state, epsilon.N);
	mpz_urandomm(u, state, epsilon.N);
	mpz_urandomm(beta, state, epsilon.N);
#if defined(DEBUG)
    gmp_printf("t: %Zd \n", t);
    gmp_printf("u: %Zd \n", u);
    gmp_printf("beta: %Zd \n", beta);
    gmp_printf("N: %Zd \n", epsilon.N);
#endif
	// L'
	AS Spl_incline;
	for(int i=0; i<AttributeSetSize; ++i){
		Spl_incline.LS[i] = (S_incline.LS[i] + 1) % attributes[i].num_values;
#if defined(DEBUG)
    printf("AttributeName: %s \nAttributeSetValue: %s \n", attributes[i].attribute_name, attributes[i].attribute_values[Spl_incline.LS[i]]);
#endif
	}

	// for calculate
	element_t g2alpha, g3alpha;
	element_init_G1(g2alpha, epsilon.pairing);
	element_init_G1(g3alpha, epsilon.pairing);
	// need to revise setup for alpha
	element_pow_zn(g2alpha, epsilon.g2, S.alpha);
	element_pow_zn(g3alpha, epsilon.g3, S.alpha);

	element_init_G1(ts.K01, epsilon.pairing);
	element_pow_zn(ts.K01, epsilon.g2_a, t);
	element_mul(ts.K01, ts.K01, g2alpha);	// K0,1
    element_init_G1(ts.K02, epsilon.pairing);
	element_pow_zn(ts.K02, epsilon.g2, t)	// K0,2
    element_init_G1(ts.K11, epsilon.pairing);
	element_pow_zn(ts.K11, epsilon.g3_a, t);
	element_mul(ts.K11, ts.K11, g3alpha);	// K1,1
    element_init_G1(ts.K12, epsilon.pairing);
	element_pow_zn(ts.K12, epsilon.g3, t)	// K1,2
    for(int i=0; i<AttributeSetSize; ++i){
		element_init_G1(ts.K03, epsilon.pairing);
	    element_init_G1(ts.K13, epsilon.pairing);
		if(delta.b = 0){
			epsilon.pub_info_H(&hash, str, size, mpz_get_ui(gamma));
			element_from_hash(ts.K03, S.hash2pl(S.LS[i]), SHA384_DIGEST_LENGTH);
			element_pow_zn(ts.K03, ts.K03, t);	// K0,3,i
			element_from_hash(ts.K13, S.hash3pl(Spl.LS[i]), SHA384_DIGEST_LENGTH);
			element_pow_zn(ts.K13, ts.K13, t);	// K1,3,i
		}else{
			element_from_hash(ts.K03, S.hash3pl(S.LS[i]), SHA384_DIGEST_LENGTH);
			element_pow_zn(ts.K03, ts.K03, t);	// K0,3,i
			element_from_hash(ts.K13, S.hash2pl(Spl.LS[i]), SHA384_DIGEST_LENGTH);
			element_pow_zn(ts.K13, ts.K13, t);	// K1,3,i
		}
	}
    element_init_G1(ts.K04, epsilon.pairing);
	element_pow_mpz(ts.K04, epsilon.g2, beta);	// K0,4
    element_init_G1(ts.K05, epsilon.pairing);
	element_pow_mpz(ts.K05, epsilon.g2, u);	// K0,5
    element_init_G1(ts.K14, epsilon.pairing);
	element_pow_mpz(ts.K14, epsilon.g3, beta);	// K1,4
    element_init_G1(ts.K15, epsilon.pairing);
	element_pow_mpz(ts.K15, epsilon.g3, u);	// K1,5

	// delta U, fakekey
	element_init_G1(delta.U, pairing);
	if(delta.b == 0){
		element_pow_mpz(delta.U, epsilon.g3, beta);
		element_pow_mpz(fakekey.fakekey, epsilon.g2, beta);
	}else{
		element_pow_mpz(delta.U, epsilon.g2, beta);
		element_pow_mpz(fakekey.fakekey, epsilon.g3, beta);
	}
	element_pow_mpz(delta.U, delta.U, u);
	element_pow_mpz(fakekey.fakekey, fakekey.fakekey, u);
	element_mul(delta.U, S.g1_alpha, delta.U);
	element_mul(fakekey.fakekey, S.g1_alpha, fakekey.fakekey);

	// clear gmp state, mpz, element
	gmp_lib.gmp_randclear(state);
	gmp_lib.mpz_clears(t, u, beta, null);
	element_clear(g2alpha);
	element_clear(g3alpha);
}

char InputValueSet[AttributeSetSize] = {"City", "Cardiologist", "Famale", "105-2568"};

int main() {
	PUB_INFO epsilon;
	SECRET S;
	AS S_incline;
	Ts ts;
	Delta delta;
	Fakekey fakekey;
	for(int i=0; i<AttributeSetSize; ++i){
		int cnt = 0;
		while(InputValueSet[i] != attribute[i].attribute_values[cnt]){
			if(cnt++ == attribute[i].num_values){
				break;
			}
		}
		S_incline.LS[i] = cnt;
#if defined(DEBUG)
    printf("AttributeName: %s \nAttributeSetValue: %s \n", attributes[i].attribute_name, attributes[i].attribute_values[S_incline.LS[i]]);
#endif
	}
	trapgen(epsilon, S, S_incline, ts, delta, fakekey);
}