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
  char* attributeName[] = {"Hospital", "Department", "Sex", "Doctor number"};
  char* accessString = "((A & D) | ((A & B) & C))"; 
  Policy A = convertToPolicy(accessString);
  char* T[] = {"Central hospital", "311-4321", "City hospital", "Genetics", "Male"};
  
  // For trapgen
  char* IS[2][3] = {{"Hospital", "Sex", "Doctor number"}, {"Hospital", "Sex", "Department"}};
  char* LS[2][3] = {{"Central hospital", "Male", "311-4321"}, {"City hospital", "Female", "Genetics"}};

  /*** Initialization End ***/

  
  /*** Execution Begin***/

  setup(lambda);
  Enc(epsilon, message, A, T);
  trapgen(epsilon, S, LS[i]);
  test(epsilon, C, ts);
  test2(epsilon, delte, C_prime);
  dec(epsilon, S, C_prime);

  /*** Execution End***/
  return 0;
}
