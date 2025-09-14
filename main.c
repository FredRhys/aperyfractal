#include <gmp.h>
#include <mpfr.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// the rounding type used by this program; used when initializing MPFR variables
#define R MPFR_RNDN
// used as null pointer by the program; used to terminate lists of MPFR variables in variadic functions
#define mpfr_null (mpfr_ptr) 0

int str_to_int(char *str) {
  return (int) strtol(str, 0, 10);
}

void mpfr_init_lists(int width, int prec, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width]) {
  for (int i = 0; i < width; i++) {
    mpfr_inits2(prec, a[i], b[i], lcm[i], mpfr_null);
  }
}

void mpfr_clear_lists(int width, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width]) {
  for (int i = 0; i < width; i++) {
    mpfr_clears(a[i], b[i], lcm[i], mpfr_null);
  }
}

void delta(mpfr_t a1, mpfr_t a0, mpfr_t b1, mpfr_t b0) {

}

int P(int n) {
  return 34 * pow(n, 3) + 51 * pow(n, 2) + 27 * n + 5;
}

// sets rop to (P(n) * u1 - n^3 * u0)/ (n+1)^3
void mpfr_rec(int n, mpfr_t u2, mpfr_t u1, mpfr_t u0, mpfr_t h) {
  mpfr_mul_si(u2, u1, P(n), R);
  mpfr_mul_si(h, u0, (int)pow(n, 3), R);
  mpfr_sub(u2, u2, h, R);
  mpfr_div_d(u2, u2, (int)pow(n+1, 3), R);
}

void mpfr_gcd(int prec, mpfr_t rop, mpfr_t a, int b, mpfr_t h1) {
  mpfr_t h2; //second helper variable
  mpfr_init2(h2, prec);
  mpfr_set(rop, a, R);
  mpfr_set_si(h2, b, R);

  while (!mpfr_zero_p(h2)) {
    mpfr_set(h1, h2, R); // t <- b
    mpfr_fmod(h2, rop, h1, R); // b <- a % b
    mpfr_set(rop, h1, R); // a <- t
  }
  mpfr_clear(h2);
}

void mpfr_lcm(mpfr_t lcm2, mpfr_t lcm1, int n, mpfr_t h) {


}

void mpfr_set_initial_vals(int width, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width]) {
  // based off previous calculations
  mpfr_set_si(a[0], 0, R);
  mpfr_set_si(a[1], 6, R);
  mpfr_set_si(b[0], 1, R);
  mpfr_set_si(b[1], 5, R);
  mpfr_set_si(lcm[1], 1, R);
}

void mpfr_fill_seqs(int width, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width], mpfr_t h) {
  for (int i = 1; i < width - 1; i++) {
    mpfr_rec(i, a[i+1], a[i], a[i-1], h);
    mpfr_rec(i, b[i+1], b[i], b[i-1], h);
    mpfr_lcm(lcm[i+1], lcm[i], i, h);
  }
}

int mainloop(int width, int prec) {
  // MPFR variables storing the sequences in Apery's sequences
  // h is a helper variable
  mpfr_t a[width], b[width], lcm[width], h;

  // Initialize all the above with `prec` bits of precision
  mpfr_init_lists(width, prec, a, b, lcm);
  mpfr_init2(h, prec);

  // set the initial values for the sequences
  mpfr_set_initial_vals(width, a, b, lcm);

  // fill the sequences
  mpfr_fill_seqs(width, a, b, lcm, h);

  // the mainloop
  for (int i = 0; i < width; i++) {
    mpfr_printf("%.16Rf\n", b[i]);
    for (int j = i+1; j <= width; j++) {      
    }
  }

  //clear the memory used by MPFR
  mpfr_clear_lists(width, a, b, lcm);
  mpfr_clear(h);
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