#include "numtheory.h"
#include "randstate.h"

// Finds the greatest common divisor through the Euclid's algorithm.
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t temp, a_temp, b_temp;
    mpz_inits(temp, a_temp, b_temp, NULL);
    mpz_set(a_temp, a);
    mpz_set(b_temp, b);

    while (mpz_cmp_ui(b_temp, 0) != 0) {
        mpz_set(temp, b_temp);
        mpz_mod(b_temp, a_temp, b_temp);
        mpz_set(a_temp, temp);
    }
    mpz_set(d, a_temp);
    mpz_clears(temp, a_temp, b_temp, NULL);
}

// Calculates the modular inverse.
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r, r_inv, q, t, t_inv, temp, temp1;
    mpz_inits(r, r_inv, q, t, t_inv, temp, temp1, NULL);
    // (r, r_inv) <- (n, a)
    mpz_set(r, n);
    mpz_set(r_inv, a);
    // (t, t_inv) <- (0, 1)
    mpz_set_si(t_inv, 1);

    while (mpz_cmp_ui(r_inv, 0) != 0) {
        // q <- (floor(r/r_inv))
        mpz_fdiv_q(q, r, r_inv);

        // (r, r_inv) <- (r_inv, r - q * r_inv)
        mpz_set(temp, r);
        mpz_set(r, r_inv);
        mpz_mul(temp1, q, r_inv);
        mpz_sub(temp1, temp, temp1);
        mpz_set(r_inv, temp1);

        // (t, t_inv) <- (t_inv, t - q * t_inv)
        mpz_set(temp, t);
        mpz_set(t, t_inv);
        mpz_mul(temp1, q, t_inv);
        mpz_sub(temp1, temp, temp1);
        mpz_set(t_inv, temp1);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(t, 0);
    } else if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(i, t);
    mpz_clears(r, r_inv, q, t, t_inv, temp, temp1, NULL);
}

// Computes fast modular exponentation.
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t p, exp_replace;
    mpz_inits(p, exp_replace, NULL);
    mpz_set(p, base);
    mpz_set(exp_replace, exponent);
    mpz_set_si(out, 1);

    while (mpz_cmp_ui(exp_replace, 0) > 0) {
        if (mpz_odd_p(exp_replace)) {
            mpz_mul(out, out, p);
            mpz_mod(out, out, modulus);
        }
        mpz_pow_ui(p, p, 2);
        mpz_mod(p, p, modulus);
        mpz_fdiv_q_ui(exp_replace, exp_replace, 2);
    }
    mpz_clears(p, exp_replace, NULL);
}

// Credit: Eugene
// Computes a strong pseudoprime test using the Miller-Rabin test.
bool is_prime(mpz_t n, uint64_t iters) {
    // These first two if statements check for edge cases: even number, 1, 2, 3.
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true;
    }
    if (mpz_cmp_ui(n, 1) == 0 || mpz_divisible_ui_p(n, 2) != 0) {
        return false;
    }
    mpz_t r, s, a, y, j, temp, temp1, exponent;
    mpz_inits(r, s, a, y, j, temp, temp1, exponent, NULL);
    mpz_sub_ui(r, n, 1);
    while (mpz_divisible_ui_p(r, 2) != 0) {
        mpz_add_ui(s, s, 1);
        mpz_fdiv_q_ui(r, r, 2);
    }
    // Set values of auxilary variables
    mpz_sub_ui(temp, n, 1);
    mpz_sub_ui(temp1, s, 1);
    mpz_set_si(exponent, 2);

    for (uint64_t i = 1; i <= iters; i++) {
        do {
            mpz_urandomm(a, state, temp);
        } while (mpz_cmp_ui(a, 2) < 0);
        pow_mod(y, a, r, n);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, temp) != 0) {
            mpz_set_si(j, 1);
            while ((mpz_cmp(j, s) < 0 || mpz_cmp(j, temp1) == 0) && mpz_cmp(y, temp) != 0) {
                pow_mod(y, y, exponent, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(r, s, a, y, j, temp, temp1, exponent, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, temp) != 0) {
                mpz_clears(r, s, a, y, j, temp, temp1, exponent, NULL);
                return false;
            }
        }
    }
    mpz_clears(r, s, a, y, j, temp, temp1, exponent, NULL);
    return true;
}

// Generates a prime number with specified number of bits.
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    do {
        mpz_urandomb(p, state, bits);
    } while ((uint64_t) mpz_sizeinbase(p, 2) < bits || is_prime(p, iters) == false);
}
