#include <gmp.h>
#include <mpfr.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// the rounding type used by this program; used when initializing MPFR variables
#define R MPFR_RNDN
// used as null pointer by the program; used to terminate lists of MPFR variables in variadic functions
#define mpfr_null (mpfr_ptr) 0

void nCr(mpfr_t rop, int n, int r) {
  const int k = r < n - r ? n - r + 1 : r + 1;
  mpfr_set_si(rop, 1, R);
  for (int i = n; i >= k; i--) {
    mpfr_mul_si(rop, rop, i, R);
    mpfr_div_si(rop, rop, i - n + 1, R);
  }
}

void binom_sum(mpfr_t rop, int n, int m, mpfr_t h) {
  nCr(rop, n, m);
  nCr(h, n + m, m);
  mpfr_mul(rop, rop, h, R);
}

void binom_sum_sq(mpfr_t rop, int n, int m, mpfr_t h) {
  binom_sum(rop, n, m, h);
  mpfr_sqr(rop, rop, R);
}

void apery_aux(mpfr_t rop, int n, int k, mpfr_t h1, mpfr_t h2, mpfr_t h3) {
  mpfr_set_zero(rop, 0);
  mpfr_set_zero(h1, 0);
  for (int l = 1; l <= n; l++) {
    mpfr_set_si(h1, l, R);
    mpfr_pow_si(h1, h1, -3, R);
    mpfr_add(rop, rop, h1, R);
  } //1
  mpfr_set_zero(h1, 0);
  for (int m = 1; m <= k; m++) {
    mpfr_set_si(h1, m % 2 == 0 ? -1 : 1, R);
    binom_sum(h2, n, m, h3);
    mpfr_set_si(h3, m, R);
    mpfr_pow_si(h3, h3, 3, R);
    mpfr_mul(h2, h2, h3, R);
    mpfr_mul_si(h2, h2, 2, R);
    mpfr_div(h1, h1, h2, R);
  }
  mpfr_add(rop, rop, h1, R);
}

void dir_a(mpfr_t rop, int n, mpfr_t h1, mpfr_t h2, mpfr_t h3, mpfr_t h4, mpfr_t h5) {
  mpfr_set_zero(rop, 0);
  for (int k = 0; k <= n; k++) {
    binom_sum_sq(h1, n, k, h2);
    apery_aux(h2, n, k, h3, h4, h5);
    mpfr_mul(h1, h1, h2, R);
    mpfr_add(rop, rop, h1, R);
  }
}

void dir_b(mpfr_t rop, int n, mpfr_t h1, mpfr_t h2) {
  mpfr_set_zero(rop, 0);
  for (int k = 0; k <= n; k++) {
    binom_sum_sq(h1, n, k, h2);
    mpfr_add(rop, rop, h1, R);
  }
}

int str_to_int(char *str) {
  return (int) strtol(str, 0, 10);
}

void mpfr_clear_lists(int width, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width]) {
  for (int i = 0; i < width; i++) {
    mpfr_clears(a[i], b[i], lcm[i], mpfr_null);
  }
}

void delta(mpfr_t rop, mpfr_t a1, mpfr_t a0, mpfr_t b1, mpfr_t b0, mpfr_t lcm1, mpfr_t lcm0,
  mpfr_t h1, mpfr_t h2, mpfr_t h3) {
  mpfr_pow_si(h1, lcm1, 3, R);
  mpfr_mul_si(h1, h1, 2, R);
  mpfr_pow_si(h2, lcm0, 3, R);
  mpfr_mul_si(h2, h2, 2, R);
  mpfr_fmms(rop, h1, a1, h2, a0, R);
  mpfr_fmms(h3, h1, b1, h2, b0, R);
  mpfr_div(rop, rop, h3, R);
}

void P(mpfr_t rop, int n, mpfr_t h) {
  mpfr_set_si(rop, n, R);
  mpfr_pow_si(rop, rop, 3, R);
  mpfr_mul_si(rop, rop, 34, R);

  mpfr_set_si(h, n, R);
  mpfr_sqr(h, h, R);
  mpfr_mul_si(h, h, 51, R);

  mpfr_add(rop, rop, h, R);

  mpfr_set_si(h, n, R);
  mpfr_mul_si(h, h, 27, R);

  mpfr_add(rop, rop, h, R);
  mpfr_add_si(rop, rop, 5, R);
}

// sets rop to (P(n) * u1 - n^3 * u0)/ (n+1)^3
void mpfr_rec(int n, mpfr_t u2, mpfr_t u1, mpfr_t u0, mpfr_t h1, mpfr_t h2) {
  P(h2, n, h1);
  mpfr_mul(u2, u1, h2, R);
  
  mpfr_set_si(h2, n, R);
  mpfr_pow_si(h2, h2, 3, R);
  mpfr_mul(h1, u0, h2, R);
  mpfr_sub(u2, u2, h1, R);

  mpfr_set_si(h2, n+1, R);
  mpfr_pow_si(h2, h2, 3, R);
  mpfr_div(u2, u2, h2, R);
}

void mpfr_gcd(mpfr_t rop, mpfr_t a, int b, mpfr_t h1, mpfr_t h2) {
  mpfr_set(rop, a, R);
  mpfr_set_si(h2, b, R);

  while (!mpfr_zero_p(h2)) {
    mpfr_set(h1, h2, R); // t <- b
    mpfr_fmod(h2, rop, h2, R); // b <- a % b
    mpfr_set(rop, h1, R); // a <- t
  }
}

void mpfr_lcm(int n, mpfr_t lcm2, mpfr_t lcm1, mpfr_t h1, mpfr_t h2, mpfr_t h3) {
  mpfr_mul_si(lcm2, lcm1, n + 1, R);
  mpfr_gcd(h1, lcm1, n + 1, h2, h3);
  mpfr_div(lcm2, lcm2, h1, R);
}

void mpfr_set_initial_vals(int start, int width, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width], mpfr_t h1, mpfr_t h2, mpfr_t h3, mpfr_t h4, mpfr_t h5) {
  // based off previous calculations
  dir_a(a[start], start, h1, h2, h3, h4, h5);
  dir_a(a[start + 1], start + 1, h1, h2, h3, h4, h5);
  dir_b(b[start], start, h1, h2);
  dir_b(b[start + 1], start + 1, h1, h2);
  mpfr_set_si(lcm[1], 1, R);
}

void mpfr_init_lists(int start, int width, int prec, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width], int init) {
  for (int i = start; i < width; i++) {
    if (init) {
      mpfr_inits2(prec, a[i], b[i], lcm[i], mpfr_null);
    } else {
      mpfr_set_prec(a[i], prec);
      mpfr_set_prec(b[i], prec);
      mpfr_set_prec(lcm[i], prec);
    }
  }
}

void set_list_precs(int start, int width, int prec, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width], 
mpfr_t h1, mpfr_t h2, mpfr_t h3, mpfr_t h4, mpfr_t h5, mpfr_t delta_res, mpfr_t z3, int init) {
  mpfr_init_lists(start, width, prec, a, b, lcm, init);
  if (init) {
    mpfr_inits2(prec, h1, h2, h3, h4, h5, delta_res, z3, mpfr_null);
  } else {
    mpfr_set_prec(h1, prec);
    mpfr_set_prec(h2, prec);
    mpfr_set_prec(h3, prec);
    mpfr_set_prec(h4, prec);
    mpfr_set_prec(h5, prec);
    mpfr_set_prec(delta_res, prec);
    mpfr_set_prec(z3, prec);
  }
}

void mpfr_fill_seqs(int start, int width, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width],
  mpfr_t h1, mpfr_t h2, mpfr_t h3) {
  for (int i = start; i < width - 1; i++) {
    mpfr_rec(i, a[i+1], a[i], a[i-1], h1, h2);
    mpfr_rec(i, b[i+1], b[i], b[i-1], h1, h2);
    mpfr_lcm(i, lcm[i+1], lcm[i], h1, h2, h3);
  }
}

void setup(int start, int width, int prec, mpfr_t a[width], mpfr_t b[width], mpfr_t lcm[width], 
mpfr_t h1, mpfr_t h2, mpfr_t h3, mpfr_t h4, mpfr_t h5, mpfr_t delta_res, mpfr_t z3, int init) {
  // Initialize all the above with `prec` bits of precision
  set_list_precs(start, width, prec, a, b, lcm, h1, h2, h3, h4, h5, delta_res, z3, init);

  // set the initial values for the sequences
  mpfr_set_initial_vals(start, width, a, b, lcm, h1, h2, h3, h4, h5);
  mpfr_zeta_ui(z3, 3, R);

  // fill the sequences
  mpfr_fill_seqs(1, width, a, b, lcm, h1, h2, h3);
}

int num_digits(int x) {
  if (-10 < x && x < 10)
    return 1;
  return 1 + num_digits(x/10);
}

void save_point(FILE *fpt, int linelen, int width, int i, int j) {
  const int pad = linelen - num_digits(i) - num_digits(j);
	fprintf(fpt, "%*.*s%d,%d\n", 0, pad, "00000000000000000000000000000", i, j);
}

int has_l_adj_p(FILE *fpt, int linelen, int x, int y) {
  fseek(fpt, -linelen-2, SEEK_END);
  char line[linelen+2];
  fgets(line, linelen+2, fpt);
  printf("%s\n", line);
  fseek(fpt, 0, SEEK_END); //resets pointer
  return 0;
}

int mainloop(int width, int prec) {
  // MPFR variables storing the sequences in Apery's sequences
  // h's are helper variables, z3 stores Apery's constant
  mpfr_t a[width], b[width], lcm[width], h1, h2, h3, h4, h5, delta_res, z3;

  const int linelen = 2 * num_digits(width) + 1;
  int vcounter = 0; // counts how many points are in the vertical line

  FILE *fpt;
	fpt = fopen("output.csv", "w+");
	fprintf(fpt, "%*.*sx,y\n", 0, linelen - 2, "00000000000000000000000000000");
  setup(0, width, prec, a, b, lcm, h1, h2, h3, h4, h5, delta_res, z3, 1);
  

  // the mainloop
  for (int i = 0; i < width - 1; i++) {
    for (int j = i + 1; j <= width - 1; j++) {
      delta(delta_res, a[j], a[i], b[j], b[i], lcm[j], lcm[i], h1, h2, h3);
      if (mpfr_equal_p(delta_res, z3)) {
        prec *= 2;
        setup((i < j ? i : j) - 1, width, prec, a, b, lcm, h1, h2, h3, h4, h5, delta_res, z3, 0);
        j--;
        //checks also if there is a point to the immediate left, or left-and-down-one
      }
      if (mpfr_cmp(delta_res, z3) < 0) {
        if (vcounter == 1) {
          if (has_l_adj_p(fpt, linelen, i, j)) {
            
          }
        }
        save_point(fpt, linelen, width, i, j);
        vcounter++;
      }
      else {
        vcounter = 0;
      }
    }
  }

  //clear the memory used by MPFR
  mpfr_clear_lists(width, a, b, lcm);
  mpfr_clears(h1, h2, h3, h4, h5, delta_res, z3, mpfr_null);
  mpfr_free_cache();
  fclose(fpt);
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
