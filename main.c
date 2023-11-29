#include <stdio.h>
#include <gmp.h>
#include "policy/createPolicy.h"
#include "setup.h"
#include "enc.h"
#include "trapgen.h"
#include "test.h"
#include "test2.h"
#include "dec.h"


int main(void) {
  /*** Initialization Begin ***/

  // For setup
  mpz_t lambda;
  mpz_init_set_ui(lambda, 256);

  // For enc
  char* message = "Long long way to go...";
  char* attributeName[] = {"Hospital", "Department", "Sex", "Doctor_number"};
  char* accessString = "((A & D) | ((A & B) & C))"; 
  Policy A = convertToPolicy(accessString);
  char* T[] = {"Central", "311-4321", "City", "Genetics", "Male"};

  // For trapgen
  char* IS[2][3] = {{"Hospital", "Sex", "Doctor_number"}, {"Hospital", "Sex", "Department"}};
  char* LS[2][3] = {{"Central", "Male", "311-4321"}, {"City", "Female", "Genetics"}};

  /*** Initialization End ***/


  /*** Execution Begin***/

  setup(lambda);
  enc(epsilon, message, A, T);
  ATTRIBUTESET aSet;
  for(int i = 0; i < 2; ++i) {
	aSet.IS = IS[i];
	aSet.LS = LS[i];
	aSet.setSize = 3;
	puts("Here");
	trapgen(epsilon, S, aSet);
	puts("Here2");
	test(epsilon, C, ts);
	puts("Here");
	if(!C_prime.VALID) {
	  puts("Perp");
	} else {
	  if(!test2(epsilon, delta, C_prime)) {
		puts("False positive");
	  } else {
		Dec(epsilon, S, C_prime);
	  }
	}
  }

  /*** Execution End***/
  return 0;
}
