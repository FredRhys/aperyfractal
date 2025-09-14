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

void mpfr_rec(mpfr_t rop, mpfr_t u1, mpfr_t u0) {

}

void mpfr_lcm(mpfr_t rop, mpfr_t lcm1, int n) {

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

void mpfr_set_initial_vals(int width, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width]) {
  // based off previous calculations
  mpfr_set_si(a[0], 0, R);
  mpfr_set_si(a[1], 5, R);
  mpfr_set_si(b[0], 1, R);
  mpfr_set_si(b[1], 6, R);
  mpfr_set_si(lcm[1], 1, R);
}

void mpfr_fill_seqs(int width, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width]) {
  for (int i = 2; i < width; i++) {
    mpfr_rec(a[i+1], a[i], a[i-1]);
    mpfr_rec(b[i+1], b[i], b[i-1]);
    mpfr_lcm(lcm[i+1], lcm[i], i);
  }
}

int mainloop(int width, int prec) {
  // MPFR variables storing the sequences in Apery's sequences
  mpfr_t a[width], b[width], lcm[width];

  // Initialize all the above with `prec` bits of precision
  mpfr_init_lists(width, prec, a, b, lcm);

  // set the initial values for the sequences
  mpfr_set_initial_vals(width, a, b, lcm);

  // fill the sequences
  mpfr_fill_seqs(width, a, b, lcm);

  // the mainloop
  for (int i = 0; i <= width; i++) {
    for (int j = i+1; j <= width; j++) {      
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