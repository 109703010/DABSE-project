#ifndef TRAPGEN_H
#define TRAPGEN_H
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#include <pbc/pbc.h>
#include <time.h>
#include "setup.h"

void trapgen(PUB_INFO epsilon, SECRET S, char** InputValueSet);

// for attribute set index & value
#define ATTRIBUTESETSIZE 4

typedef struct Trapdoor{
	element_t K01;
	element_t K02;
	element_t K11;
	element_t K12;
	element_t K03[ATTRIBUTESETSIZE];
	element_t K13[ATTRIBUTESETSIZE];
	element_t K04;
	element_t K05;
	element_t K14;
	element_t K15;
}TRAPDOOR;

TRAPDOOR ts;

typedef struct Delta{
	int b;
	element_t U;
}DELTA;

DELTA delta;

typedef struct Fakekey{
	element_t fakekey;
}FAKEKEY;

FAKEKEY fakekey;

typedef struct AttributeSet {
    char* attribute_name;
    char* attribute_values[5];
    int num_values;
}ATTRIBUTESET;

ATTRIBUTESET attributes[ATTRIBUTESETSIZE];  // Assuming you have ATTRIBUTESETSIZE attributes

void trapgen(PUB_INFO epsilon, SECRET S, char** InputValueSet){

	size_t numElements = sizeof(InputValueSet) / sizeof(InputValueSet[0]);
	int S_incline[numElements];

	// Hospital attribute
    attributes[0].attribute_name = "Hospital";
    attributes[0].attribute_values[0] = "City";
    attributes[0].attribute_values[1] = "People";
    attributes[0].attribute_values[2] = "Central";
    attributes[0].attribute_values[3] = "Provincial";
    attributes[0].attribute_values[4] = "";  // You can replace this with the default value
    attributes[0].num_values = 5;

    // Department attribute
    attributes[1].attribute_name = "Department";
    attributes[1].attribute_values[0] = "Cardiologist";
    attributes[1].attribute_values[1] = "Stomatology";
    attributes[1].attribute_values[2] = "Neurology";
    attributes[1].attribute_values[3] = "Genetics";
    attributes[1].attribute_values[4] = "";  // You can replace this with the default value
    attributes[1].num_values = 5;

    // Sex attribute
    attributes[2].attribute_name = "Sex";
    attributes[2].attribute_values[0] = "Male";
    attributes[2].attribute_values[1] = "Female";
    attributes[2].attribute_values[2] = "";  // You can replace this with the default value
    attributes[2].num_values = 3;

    // Doctor number attribute
    attributes[3].attribute_name = "Doctor number";
    attributes[3].attribute_values[0] = "105-2568";
    attributes[3].attribute_values[1] = "021-6932";
    attributes[3].attribute_values[2] = "231-6421";
    attributes[3].attribute_values[3] = "311-4321";
    attributes[3].attribute_values[4] = "";  // You can replace this with the default value
    attributes[3].num_values = 5;

	for(int i=0; i<ATTRIBUTESETSIZE; ++i){
		int cnt = 0;
		while(InputValueSet[i] != attributes[i].attribute_values[cnt]){
			if(cnt++ == attributes[i].num_values){
				break;
			}
		}
		S_incline[i] = cnt;
	}

	// Create, initialize, and seed a new random number generator.
	gmp_randstate_t state;
	gmp_randinit_default(state);
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

	// L'
	int Spl_incline[ATTRIBUTESETSIZE];
	for(int i=0; i<ATTRIBUTESETSIZE; ++i){
		Spl_incline[i] = (S_incline[i] + 1) % attributes[i].num_values;
	}

	// for calculate
	element_t g2alpha, g3alpha;
	element_init_G1(g2alpha, epsilon.pairing_of_G);
	element_init_G1(g3alpha, epsilon.pairing_of_G);
	// need to revise setup for alpha
	element_pow_mpz(g2alpha, epsilon.g2, S.alpha);
	element_pow_mpz(g3alpha, epsilon.g3, S.alpha);

	element_init_G1(ts.K01, epsilon.pairing_of_G);
	element_pow_mpz(ts.K01, epsilon.g2_a, t);
	element_mul(ts.K01, ts.K01, g2alpha);	// K0,1
    element_init_G1(ts.K02, epsilon.pairing_of_G);
	element_pow_mpz(ts.K02, epsilon.g2, t);	// K0,2
    element_init_G1(ts.K11, epsilon.pairing_of_G);
	element_pow_mpz(ts.K11, epsilon.g3_a, t);
	element_mul(ts.K11, ts.K11, g3alpha);	// K1,1
    element_init_G1(ts.K12, epsilon.pairing_of_G);
	element_pow_mpz(ts.K12, epsilon.g3, t);	// K1,2
    for(int i=0; i<ATTRIBUTESETSIZE; ++i){
		element_init_G1(ts.K03[i], epsilon.pairing_of_G);
	    element_init_G1(ts.K13[i], epsilon.pairing_of_G);
		if(delta.b == 0){
			S.H_2_(ts.K03[i], attributes[i].attribute_values[S_incline[i]], strlen(attributes[i].attribute_values[S_incline[i]]), epsilon.g2);
			element_pow_mpz(ts.K03[i], ts.K03[i], t);	// K0,3,i
			S.H_2_(ts.K13[i], attributes[i].attribute_values[Spl_incline[i]], strlen(attributes[i].attribute_values[Spl_incline[i]]), epsilon.g3);
			element_pow_mpz(ts.K13[i], ts.K13[i], t);	// K1,3,i
		}else{
			S.H_2_(ts.K03[i], attributes[i].attribute_values[S_incline[i]], strlen(attributes[i].attribute_values[S_incline[i]]), epsilon.g3);
			element_pow_mpz(ts.K03[i], ts.K03[i], t);	// K0,3,i
			S.H_2_(ts.K03[i], attributes[i].attribute_values[Spl_incline[i]], strlen(attributes[i].attribute_values[Spl_incline[i]]), epsilon.g2);
			element_pow_mpz(ts.K13[i], ts.K13[i], t);	// K1,3,i
		}
	}
    element_init_G1(ts.K04, epsilon.pairing_of_G);
	element_pow_mpz(ts.K04, epsilon.g2, beta);	// K0,4
    element_init_G1(ts.K05, epsilon.pairing_of_G);
	element_pow_mpz(ts.K05, epsilon.g2, u);	// K0,5
    element_init_G1(ts.K14, epsilon.pairing_of_G);
	element_pow_mpz(ts.K14, epsilon.g3, beta);	// K1,4
    element_init_G1(ts.K15, epsilon.pairing_of_G);
	element_pow_mpz(ts.K15, epsilon.g3, u);	// K1,5

	// delta U, fakekey
	element_init_G1(delta.U, epsilon.pairing_of_G);
	element_init_G1(fakekey.fakekey, epsilon.pairing_of_G);
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
	gmp_randclear(state);
	mpz_clears(t, u, beta, NULL);
	element_clear(g2alpha);
	element_clear(g3alpha);
}

#if defined(DEBUG)
int main() {
	mpz_t lambda;
    mpz_init_set_ui(lambda, 256);
    setup(lambda);

	char* InputValueSet[ATTRIBUTESETSIZE];
	InputValueSet[0] = "City";
	InputValueSet[1] = "Cardiologist";
	InputValueSet[2] = "Male";
	InputValueSet[3] = "105-2568";

	trapgen(epsilon, S, InputValueSet);
}
#endif
#endif