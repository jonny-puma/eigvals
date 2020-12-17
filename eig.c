#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef DARWIN
#include <Accelerate/Accelerate.h>
#endif

/* dgeev from LAPACK */
#ifdef DARWIN
extern void dgeev(char* jobvl, char* jobvr, int* n, double* a,
                  int* lda, double* wr, double* wi, double* vl, int* ldvl,
                  double* vr, int* ldvr, double* work, int* lwork, int* info);
#else
extern void dgeev_(char* jobvl, char* jobvr, int* n, double* a,
                   int* lda, double* wr, double* wi, double* vl, int* ldvl,
                   double* vr, int* ldvr, double* work, int* lwork, int* info);
#endif

int main(int argc, char **argv) {
  /* Check input length */
  #ifdef DEBUG
  int s = 0;
  while (!strcmp("eigvals", argv[s++]));
	#else
	int s = 1;
	#endif
  int l = argc-s;
  int n = sqrt(l);

	#ifdef DEBUG
	printf("argc: %i\n", argc);
	printf("start: %i\n", s);
	printf("l: %i\n", l);
	printf("n: %i\n", n);
	for (int i=0; i<argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");
	#endif

  if (n*n != l) {
    fprintf(stderr, "Matrix must be square!\n");
    return 1;
  }

  /* Build matrix from input */
  double *A = malloc(sizeof(double)*l);
  for (int i=0; i<l; i++) {
    A[i] = atof(argv[s+i]);
  }

  /* Work vector for LAPACK */
  int lwork = 3*n;
  double *work = malloc(sizeof(double)*lwork);

  /* Eigenvalues to be computed by LAPACK */
  double *eigvals_r = malloc(sizeof(double)*n);
  double *eigvals_i = malloc(sizeof(double)*n);

  /* Run LAPACK DGEEV */
  int info;
  int ldvr = n;
  int ldvl = n;
  int lda = n;
  #ifdef DARWIN
  dgeev("N", "N", &n, A, &lda, eigvals_r, eigvals_i, NULL, &ldvl, NULL, &ldvr,
        work, &lwork, &info);
  #else
  dgeev_("N", "N", &n, A, &lda, eigvals_r, eigvals_i, NULL, &ldvl, NULL, &ldvr,
         work, &lwork, &info);
  #endif


  /* Check LAPACK exit status */
  int pi = n;
  if (info) {
    if (info < 0) {
      fprintf(stderr, "Input %i has illegal value\n", -info);
      pi = 0;
    } else {
      fprintf(stderr, "Eigenvalue %i did not converge\n", info);
      pi = info;
    }
  }

  /* print result */
  printf("[");
  for (int i=0; i<pi; i++) {
    if (eigvals_i[i] < 0) {
      printf("%.2f%.2fi, ", eigvals_r[i], eigvals_i[i]);
    } else {
      printf("%.2f+%.2fi, ", eigvals_r[i], eigvals_i[i]);
    }
  }
  printf("\b\b]\n");

  /* Cleanup */
  free(A);
  free(eigvals_r);
  free(eigvals_i);
  free(work);

  return info;
}
