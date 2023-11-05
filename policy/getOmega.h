#ifndef GET_OMEGA_H
#define GET_OMEGA_H
#include <stdio.h>
#include <stdlib.h>

//#define DEBUG = 1

void print_matrix(int** arr, int m, int n);
int gcd(int a, int b);
int lcm(int a, int b);
void swap(int* a, int* b);
void row_swap(int* row1, int* row2, int n);
void row_multiplication(int* row, int n, int multiplier);
void row_addition(int* row1, int* row2, int n, int multiplier);
int* Gaussian_Elimination(int** arr, int m, int n);
int** transpose(int** arr, int m, int n);
int* get_indepedent_row(int** arr, int m, int n);
int* get_omega(int** arr, int* row, int n); // Only 1 or 0 is posisble, -1 means invalid
int isValidOmega(int* omega);

#if defined(DEBUG)
int main() {
  int l = 4, n = 4;
  int* arr[l];
  for(int i = 0; i < l; ++i) {
	arr[i] = (int*)malloc(sizeof(int)*n);
	for(int j = 0; j < n; ++j) {
	  scanf("%d", &arr[i][j]);
	}
  }

  int* row = get_indepedent_row(arr, l, n);
  int* omega = get_omega(arr, row, n);

  puts("Input Matrix:");
  print_matrix(arr, l, n);
  puts("Omega:");
  print_matrix(&omega, 1, row[0]);
  puts("Corresponding i");
  int* tmpPtr = row+1;
  print_matrix(&tmpPtr, 1, row[0]);
  puts(isValidOmega(omega) ? "Valid": "Invalid");

  free(omega);
  free(row);
  for(int i = 0; i < l; ++i) {
	free(arr[i]);
  }
  return 0;
}
#endif

void print_matrix(int** arr, int m, int n) {
  for(int i = 0; i < m; ++i) {
	for(int j = 0; j < n; ++j) {
	  printf("%d ", arr[i][j]);
	}
	puts("");
  }
  puts("");
}

int gcd(int a, int b) {
  if(b == 0) {
	return a;
  }
  return gcd(b, a%b);
}

int lcm(int a, int b) {
  a = abs(a), b = abs(b);
  return a * b / gcd(a,b);
}

void swap(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void row_swap(int* row1, int* row2, int n) {
  for(int i = 0; i < n; ++i) {
	swap(row1 + i, row2 + i);
  }
}

void row_multiplication(int* row, int n, int multiplier) {
  for(int i = 0; i < n; ++i) {
	row[i] *= multiplier;
  }
}

void row_addition(int* row1, int* row2, int n, int multiplier) {
  for(int i = 0; i < n; ++i) {
	row2[i] += row1[i] * multiplier;
  }
}

int* Gaussian_Elimination(int** arr, int m, int n) {
  int* rowOrder = (int*)malloc(sizeof(int) * m);
  for(int i = 0; i < m; ++i) {
	rowOrder[i] = i;
  }
  for(int i = 0, j = 0; (i < m) && (j < n); ++i, ++j) {
	int k = i + 1;
	for(; (arr[i][j] == 0) && (j < n); ++j) {
	  for(k = i + 1; k < m; ++k) {
		if(arr[k][j] != 0) {
		  row_swap(arr[i], arr[k], n);
		  swap(rowOrder + i, rowOrder + k);
		  --j;
		  break;
		}
	  }
	}
	if(j == n) {
	  break;
	}
	for(; k < m; ++k) {
	  if(arr[k][j] != 0) {
		int multiplier = lcm(arr[i][j], arr[k][j]);
		int sign = ((arr[i][j] < 0) ^ (arr[k][j] < 0)) ? 1 : -1;
		row_multiplication(arr[k], n, multiplier / abs(arr[k][j]));
		row_addition(arr[i], arr[k], n, multiplier / abs(arr[i][j]) * sign);
	  }
	}
  }
  return rowOrder;
}

int** transpose(int** arr, int m, int n) {
  int** T = (int**)malloc(sizeof(int*) * n);
  for(int i = 0; i < n; ++i) {
	T[i] = (int*)malloc(sizeof(int) * m);
	for(int j = 0; j < m; ++j) {
	  T[i][j] = arr[j][i];
	}
  }
  return T;
}

int* get_indepedent_row(int** arr, int m, int n) {
  int **T = transpose(arr, m, n);
  // We don't need the rowOrder information
  free(Gaussian_Elimination(T, n, m));

  int* row = (int*)malloc(sizeof(int) * (n+1));
  int counter = 0;
  for(int i = 0, j = 0; i < n; ++i) {
	for(; j < m; ++j) {
	  if(T[i][j] != 0) {
		row[++counter] = j;
		break;
	  }
	}
  }
  row[0] = counter;

  for(int i = 0; i < n; ++i) {
	free(T[i]);
  }
  free(T);

  return row;
}

int* get_omega(int** arr, int* row, int n) {
  int l = row[0];
  int* omega = (int*)malloc(sizeof(int) * l);

  int* I[l + 1]; // Submatrix of A(augmenting matrix)
  for(int i = 0; i < l; ++i) {
	I[i] = arr[row[i + 1]];
  }
  I[l] = (int*)calloc(n, sizeof(int));
  I[l][0] = 1;
  int** I_tranpose = transpose(I, l + 1, n);

  int* rowOrder = Gaussian_Elimination(I_tranpose, n, l + 1);
  // Gauss-Jordan Elimination
  int flag = 1; // Record if there exist valid omegas
  for(int i = n - 1; i >= l; --i) {
	if(I_tranpose[i][l] != 0) {
	  omega[0] = -1;
	  flag = 0;
	  break;
	}
  }
  for(int i = l - 1, j = l - 1; flag && i >= 0; --i) {
	while(I_tranpose[i][j] == 0) {
	  --j;
	}
	for(int k = i - 1; k >= 0; --k) {
	  if(I_tranpose[k][j] != 0) {
		int multiplier = lcm(I_tranpose[i][j], I_tranpose[k][j]);
		int sign = ((I_tranpose[i][j] < 0) ^ (I_tranpose[k][j] < 0)) ? 1 : -1;
		row_multiplication(I_tranpose[k], l + 1, multiplier / abs(I_tranpose[k][j]));
		row_addition(I_tranpose[i], I_tranpose[k], l + 1, multiplier / abs(I_tranpose[i][j]) * sign);
	  }
	}
	omega[i] = I_tranpose[i][l] / I_tranpose[i][j];
  }

  for(int i = 0; i < l; ++i) {
	if(rowOrder[i] == i) {
	  continue;
	}
	for(int j = i; j < l; ++j) {
	  if(rowOrder[i] == i) {
		swap(omega + i, omega + j);
		swap(row + i, row + j);
		break;
	  }
	}
  }

  free(rowOrder);
  free(I[l]);
  for(int i = 0; i < n; ++i) {
	free(I_tranpose[i]);
  }
  free(I_tranpose);

  return omega;
}

int isValidOmega(int* omega) {
  if(omega[0] == -1) {
	return 0;
  }
  return 1;
}
#endif
