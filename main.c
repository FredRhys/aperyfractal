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

void delta(mpfr_t a1, mpfr_t a0, mpfr_t b1, mpfr_t b0) {

}

void mpfr_rec(mpfr_t rop, mpfr_t u2, mpfr_t u1, mpfr_t u0) {

}

void mpfr_inc_seq(mpfr_t u2, mpfr_t u1, mpfr_t u0) {
  // move up u1 and u0 in the sequence
  mpfr_set(u0, u1, R);
  mpfr_set(u1, u2, R);

  // calculate the next term in the sequence and assign it to u2
  mpfr_rec(u2, u2, u1, u0);
}

void mpfr_init_lists(int width, int prec, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width]) {
  for (int i = 0; i < width; i++) {
    mpfr_inits2(prec, a[i], b[i], lcm[i], mpfr_null);
  }
}

void mpfr_clear_lists(int width, int prec, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width]) {
  for (int i = 0; i < width; i++) {
    mpfr_clears(a[i], b[i], lcm[i], mpfr_null);
  }
}

int mainloop(int width, int prec) {
  // MPFR variables storing the sequences in Apery's sequences
  mpfr_t a[width], b[width], lcm[width];

  // Initialize all the above with `prec` bits of precision
  mpfr_init_lists(width, prec, a, b, lcm);

  // set the initial values for the sequences
  mpfr_set_si(a[0], 0, R);
  mpfr_set_si(a[1], 5, R);
  mpfr_set_si(b[0], 1, R);
  mpfr_set_si(b[1], 6, R);
  mpfr_set_si(lcm[1], 1, R);

  // the mainloop
  for (int i = 0; i <= width; i++) {
    for (int j = i+1; j <= width; j++) {
      // calculate the next terms in the sequences
      
    }
  }

  //clear the memory used by MPFR
  mpfr_clear_lists(width, prec, a, b, lcm);
  mpfr_free_cache();
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
  return 0;
}