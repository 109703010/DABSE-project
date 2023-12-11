#ifndef TRAPGEN_H
#define TRAPGEN_H
#include <time.h>
#include "setup.h"
#include "attribute/Attribute.h"
#include "attribute/AttributeSet.h"

void trapgen(PUB_INFO epsilon, SECRET S, ATTRIBUTESET attrSet);

typedef struct Trapdoor{
	element_t K01;
	element_t K02;
	element_t K11;
	element_t K12;
	element_t *K03;
	element_t *K13;
	element_t K04;
	element_t K05;
	element_t K14;
	element_t K15;
	char** IS;
	int ISSize;
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

void trapgen(PUB_INFO epsilon, SECRET S, ATTRIBUTESET attrSet){

	int numattributes = 0;
    ATTRIBUTE* attributes = setAttribute(&numattributes);
	int S_incline[attrSet.setSize];
	ts.K03 = realloc(ts.K03, attrSet.setSize * sizeof(element_t));
	ts.K13 = realloc(ts.K13, attrSet.setSize * sizeof(element_t));
	ts.IS = malloc(attrSet.setSize * sizeof(char*));
	memcpy(ts.IS, attrSet.IS, attrSet.setSize * sizeof(char*));
	ts.ISSize = attrSet.setSize;

	for(int i=0; i<attrSet.setSize; ++i){
		int cnt = 0;
		for(int j=0; j<numattributes; ++j){
			if(strcmp(attrSet.IS[i], attributes[j].attribute_name) == 0){
				for(int k=0; k<attributes[j].num_values; ++k){
					if(strcmp(attrSet.LS[i], attributes[j].attribute_values[k]) == 0){
						break;
					}
					else if(k == attributes[j].num_values - 1){
						// add attribute value to attribute Category
						// revise the num of values in attribute Category
						// need to add functions in Attribute.h
						cnt++;
						break;
					}
					cnt++;
				}
				break;
			}
			else if(j == numattributes - 1){
				// add attribute name to attribute Category
				// revise the num of attributes in attribute Category
				// need to add functions in Attribute.h
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
	int Spl_incline[attrSet.setSize];
	for(int i=0; i<attrSet.setSize; ++i){
		for(int j=0; j<numattributes; ++j){
			if(strcmp(attrSet.IS[i], attributes[j].attribute_name) == 0){
				Spl_incline[i] = S_incline[i] + (rand() % attributes[j].num_values);
				Spl_incline[i] = Spl_incline[i] % attributes[j].num_values;
				break;
			}
		}
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
    for(int i=0; i<attrSet.setSize; ++i){
		element_init_G1(ts.K03[i], epsilon.pairing_of_G);
	    element_init_G1(ts.K13[i], epsilon.pairing_of_G);
		ts.IS[i] = strdup(attrSet.IS[i]);
		for(int j=0; j<numattributes; ++j){
			if(strcmp(attrSet.IS[i], attributes[j].attribute_name) == 0){
				if(delta.b == 0){
					//printf("attribute value: %s %s\n", attrSet.LS[i], attributes[j].attribute_values[S_incline[i]]);
					S.H_2_(ts.K03[i], attributes[j].attribute_values[S_incline[i]], strlen(attributes[j].attribute_values[S_incline[i]]), epsilon.g2);
					element_pow_mpz(ts.K03[i], ts.K03[i], t);	// K0,3,i
					S.H_2_(ts.K13[i], attributes[j].attribute_values[Spl_incline[i]], strlen(attributes[j].attribute_values[Spl_incline[i]]), epsilon.g3);
					element_pow_mpz(ts.K13[i], ts.K13[i], t);	// K1,3,i
				}else{
					S.H_2_(ts.K03[i], attributes[j].attribute_values[S_incline[i]], strlen(attributes[j].attribute_values[S_incline[i]]), epsilon.g3);
					element_pow_mpz(ts.K03[i], ts.K03[i], t);	// K0,3,i
					S.H_2_(ts.K03[i], attributes[j].attribute_values[Spl_incline[i]], strlen(attributes[j].attribute_values[Spl_incline[i]]), epsilon.g2);
					element_pow_mpz(ts.K13[i], ts.K13[i], t);	// K1,3,i
				}
				break;
			}
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

	ATTRIBUTESET attrSet = {NULL, NULL, 0};
    setAttributeSet(&attrSet);

	trapgen(epsilon, S, attrSet);
	// printf("1\n");
	return 0;
}
#endif
#endif
