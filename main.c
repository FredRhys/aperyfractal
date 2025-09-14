#include <gmp.h>
#include <mpfr.h>
#include <stdlib.h>
#include <stdio.h>

// the rounding type used by this program; used when initializing MPFR variables
#define R MPFR_RNDN
// used as null pointer by the program; used to terminate lists of MPFR variables in variadic functions
#define mpfr_null (mpfr_ptr) 0

int str_to_int(char *str) {
  return (int) strtol(str, 0, 10);
}

int mainloop(int width, int prec) {
  // MPFR variables storing the three terms in Apery's `a` sequence.
  // a2 is a_{n+1}
  // a1 is a_n
  // a0 is a_{n-1}
  // Likewise for the b variables.
  mpfr_t a0, a1, a2, b0, b1, b2;
  mpfr_inits2(prec, a0, a1, a2, b0, b1, b2, mpfr_null);
  mpfr_clears(a0, a1, a2, b0, b1, b2, mpfr_null);
  return 0;
}

int main(int argnum, char **args) {
  // quit program if incorrect number of args given
  if (argnum != 3) {
    return -1;
  }
  // the width/height of the graph being plotted
  const int width = str_to_int(args[1]);

  // the precision used during MPFR's calculations
  const int prec = str_to_int(args[2]);

  // if both `width` and `prec` are valid ints, run the mainloop
  if (width > 0 && prec > 0) {
    mainloop(width, prec);
  }

  printf("%d, %d\n", width, prec);
  return 0;
}