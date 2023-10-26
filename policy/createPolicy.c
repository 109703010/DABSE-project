#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_structure/access_tree/access_tree.h"
#include "data_structure/access_tree/AT_stack.h"
#include "data_structure/access_tree/AT_queue.h"
#include "data_structure/int/int_vector.h"
#include "data_structure/char/char_stack.h"

typedef struct {
  int l, n;
  int** M;
  char* rho;
} Policy;

Policy convertToPolicy(char* policyStr);

int main(void) {
  // Convert attribute name into symbols via 'A'+index
  char* attributeName[] = {"Hospital", "Department", "Sex", "Doctor number"};
  // Expected to be a WFF(Well-Formed Formula)
  char* accessString = "((A & D) | ((A & B) & C))"; 
  // Corresponding to Rho(It should follow the BFS order of Access Tree)
  char* T[] = {"Central Hospital", "311-4321", "City Hospital", "Genetics", "Male"};
  Policy A = convertToPolicy(accessString);

  int maxLengthOfName = 13;
  printf("Example Policy: %s\n\n", accessString);
  printf("%*s\t%*s\t%s\n", -2 * A.n, "M", -maxLengthOfName, "Rho", "T");
  for(int i = 0; i < A.l; ++i) {
	for(int j = 0; j < A.n; ++j) {
	  printf("%d ", A.M[i][j]);
	}
	printf("\t%*s", -maxLengthOfName, attributeName[A.rho[i] - 'A']);
	printf("\t%s", T[A.rho[i] - 'A']);
	puts("");
  }

  for(int i = 0; i < A.l; ++i) {
	free(A.M[i]);
  }
  free(A.M);
  free(A.rho);

  return 0;
}

Policy convertToPolicy(char* policyStr) {
  char* toPostfix(char* policyStr);
  AccessTree* buildTree(char* postfix);
  Policy LewkoWatersAlgorithm(AccessTree* AT, int l, int n);

  int l = 0, n = 1;
  for(int i = 0; policyStr[i] != '\0'; ++i) {
	if(isalpha(policyStr[i]))
	  ++l;
	if(policyStr[i] == '&')
	  ++n;
  }

  char* postfix = toPostfix(policyStr);
  AccessTree* AT = buildTree(postfix);
  Policy result = LewkoWatersAlgorithm(AT, l, n);
  
  free(postfix);
  destroyTree(AT);

  return result;
}

char* toPostfix(char* policyStr) {
  Stack stk = createStack();
  int len = strlen(policyStr);
  char* output = (char*)malloc(sizeof(char) * (len+1));
  output[len] = '\0';
  int j = 0;

  for(int i = 0; i < len; ++i) {
	char c = policyStr[i];
	if(isalpha(c)) {
	  output[j++] = c;
	} else if(c == ' ') {
	  continue;
	} else {
	  if(c == ')') {
		char opr = popStack(&stk);
		while(opr != '(') {
		  output[j++] = opr;
		  opr = popStack(&stk);
		}
	  } else {
		pushStack(&stk, c);
	  }
	}
  }
  while(!isEmptyStack(&stk)) {
	output[j++] = popStack(&stk);
  }

  destroyStack(&stk);

  return output;
}

AccessTree* buildTree(char* postfix) {
  ATStack stk = createATStack();

  for(int i = 0; postfix[i] != '\0'; ++i) {
	char c = postfix[i];
	if(isalpha(c)) {
	  AccessTree* newNode = createTree(c, NULL, NULL);
	  pushATStack(&stk, newNode);
	} else {
	  AccessTree* right = popATStack(&stk);
	  AccessTree* left = popATStack(&stk);
	  AccessTree* newNode = createTree(c, left, right);
	  pushATStack(&stk, newNode);
	}
  }

  AccessTree* result = topOfATStack(&stk);
  destroyATStack(&stk);

  return result;
}

Policy LewkoWatersAlgorithm(AccessTree* AT, int l, int n) {
  int c = 1;
  Vector* v = (Vector*)malloc(sizeof(Vector) * 1);
  *v = createVector(1);
  v -> arr[0] = 1;
  AT -> v = v;

  int** M = (int**)malloc(sizeof(int*) * l);
  char* rho = (char*)malloc(sizeof(char) * l);
  for(int i = 0; i < l; ++i) {
	M[i] = (int*)malloc(sizeof(int) * n);
  }
  int row = 0;

  ATQueue q = createATQueue();
  pushATQueue(&q, AT);
  while(!isEmptyATQueue(&q)) {
	AccessTree* cur = popATQueue(&q);

	if(cur -> data == '|') {
	  int capacity = cur -> v -> capacity;
	  *(cur -> left -> v) = createVector(capacity);
	  *(cur -> right -> v) = createVector(capacity);
	  memcpy(cur->left->v->arr, cur->v->arr, sizeof(int) * capacity);
	  memcpy(cur->right->v->arr, cur->v->arr, sizeof(int) * capacity);
	} else if(cur -> data == '&') {
	  *(cur -> left -> v) = createVector(c + 1);
	  cur -> left -> v -> arr[c] = -1;

	  *(cur -> right -> v) = createVector(c + 1);
	  memcpy(cur -> right -> v -> arr, cur -> v -> arr, cur -> v -> capacity);
	  cur -> right -> v -> arr[c] = 1;
	  ++c;
	} else {
	  memcpy(M[row], cur -> v -> arr, (cur -> v -> capacity) * sizeof(int));
	  rho[row] = cur -> data;
	  ++row;
	}

	if(cur -> left) {
	  pushATQueue(&q, cur -> left);
	}
	if(cur -> right){
	  pushATQueue(&q, cur -> right);
	}
  }
  
  destroyATQueue(&q);
  return (Policy){l, n, M, rho};
}
