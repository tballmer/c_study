#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

// This function is based on Professor Long's getopt() example from asgn2.pdf

int main(int argc, char **argv) {
    bool is_e, is_b, is_m, is_r, is_v, is_n, is_s, is_h;
    is_e = is_b = is_m = is_r = is_v = is_n = is_s = is_h = false;
    bool is_null = true;
    int opt = 0;
    double e_math = M_E;
    double pi_math = M_PI;

    // I decided to use switch statements to alter boolean values
    // then calculate and print what is needed. I got this idea from Eugene's
    // lab section. He also recommended using sets but I could not figure
    // them out in time.

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            is_e = is_b = is_m = is_r = is_v = is_n = true;
            is_null = false;
            break;
        case 'e':
            is_e = true;
            is_null = false;
            break;
        case 'b':
            is_b = true;
            is_null = false;
            break;
        case 'm':
            is_m = true;
            is_null = false;
            break;
        case 'r':
            is_r = true;
            is_null = false;
            break;
        case 'v':
            is_v = true;
            is_null = false;
            break;
        case 'n':
            is_n = true;
            is_null = false;
            break;
        case 's':
            is_s = true;
            is_null = false;
            break;
        case 'h':
            is_h = true;
            is_null = false;
            is_e = is_b = is_m = is_r = is_v = is_n = is_s = false;
            break;
        default: is_h = true; break;
        }
    }

    if (is_e) {
        double e_approx;
        e_approx = e();
        double diff = absolute(e_math - e_approx);
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e_approx, e_math, diff);
        if (is_s) {
            int e_stats;
            e_stats = e_terms();
            printf("e terms = %d\n", e_stats);
        }
    }

    if (is_r) {
        double pi_approx;
        pi_approx = pi_euler();
        double diff = absolute(pi_math - pi_approx);
        printf(
            "pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_approx, pi_math, diff);
        if (is_s) {
            int pi_stats;
            pi_stats = pi_euler_terms();
            printf("pi_euler() terms = %d\n", pi_stats);
        }
    }

    if (is_b) {
        double pi_approx;
        pi_approx = pi_bbp();
        double diff = absolute(pi_math - pi_approx);
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_approx, pi_math, diff);
        if (is_s) {
            int pi_stats;
            pi_stats = pi_bbp_terms();
            printf("pi_bbp() terms = %d\n", pi_stats);
        }
    }

    if (is_m) {
        double pi_approx;
        pi_approx = pi_madhava();
        double diff = absolute(pi_math - pi_approx);
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_approx, pi_math,
            diff);
        if (is_s) {
            int pi_stats;
            pi_stats = pi_madhava_terms();
            printf("pi_madhava() terms = %d\n", pi_stats);
        }
    }

    if (is_v) {
        double pi_approx;
        pi_approx = pi_viete();
        double diff = absolute(pi_math - pi_approx);
        printf(
            "pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_approx, pi_math, diff);
        if (is_s) {
            int pi_stats;
            pi_stats = pi_viete_factors();
            printf("pi_viete() terms = %d\n", pi_stats);
        }
    }

    if (is_n) {
        double sqrt_approx;
        double sqrt_math;
        double diff;
        for (double i = 0.0; i <= 10.0; i += 0.1) {
            sqrt_approx = sqrt_newton(i);
            sqrt_math = sqrt(i);
            diff = absolute(sqrt_math - sqrt_approx);
            printf("sqrt_newton(%7.6lf) = %16.15lf, sqrt(%7.6lf) = %16.15lf, diff = %16.15lf\n", i,
                sqrt_approx, i, sqrt_math, diff);
            if (is_s) {
                int sqrt_stats;
                sqrt_stats = sqrt_newton_iters();
                printf("sqrt_newton() terms = %d\n", sqrt_stats);
            }
        }
    }

    if (is_h || is_null) {
        printf("SYNOPSIS\n");
        printf("   A test harness for the small numerical library.\n");
        printf("\n");
        printf("USAGE\n");
        printf("   ./mathlib-test [-aebmrvnsh]\n");
        printf("\n");
        printf("OPTIONS\n");
        printf("  -a   Runs all tests.\n");
        printf("  -e   Runs e test.\n");
        printf("  -b   Runs BBP pi test.\n");
        printf("  -m   Runs Madhava pi test.\n");
        printf("  -r   Runs Euler pi test.\n");
        printf("  -v   Runs Viete pi test.\n");
        printf("  -n   Runs Newton square root tests.\n");
        printf("  -s   Print verbose statistics.\n");
        printf("  -h   Display program synopsis and usage.\n");
    }

    return 0;
}
