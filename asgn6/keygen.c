#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"

#include <gmp.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define OPTIONS "b:i:n:d:s:vh"

void print_synopsis(void);

int main(int argc, char **argv) {
    FILE *pbfile;
    FILE *pvfile;
    char *in = "rsa.pub";
    char *out = "rsa.priv";
    int opt = 0;
    uint64_t iters = 50;
    uint64_t min_bits = 256;
    uint64_t seed = time(NULL);
    bool verbose = false;
    bool help = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': min_bits = (uint64_t) atol(optarg); break;
        case 'i': iters = (uint64_t) atol(optarg); break;
        case 'n': in = optarg; break;
        case 'd': out = optarg; break;
        case 's': seed = (uint64_t) atol(optarg); break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        }
    }

    // These 3 conditionals are help and error handling.
    if (optopt || help) {
        print_synopsis();
        exit(0);
    }

    if ((pbfile = fopen(in, "w")) == NULL) {
        printf("Public key file failed to open\n");
        print_synopsis();
        exit(1);
    }
    if ((pvfile = fopen(out, "w")) == NULL) {
        printf("Private key file failed to open\n");
        print_synopsis();
        fclose(pbfile);
        exit(1);
    }

    // Credit: Eugene
    fchmod(fileno(pbfile), 0600);
    fchmod(fileno(pvfile), 0600);

    randstate_init(seed);
    mpz_t p, q, n, e, d, id, s;
    mpz_inits(p, q, n, e, d, id, s, NULL);

    rsa_make_pub(p, q, n, e, min_bits, iters);
    rsa_make_priv(d, e, p, q);

    char *name = getenv("USER");
    mpz_set_str(id, name, 62);
    rsa_sign(s, id, d, n);

    rsa_write_pub(n, e, s, name, pbfile);
    rsa_write_priv(n, d, pvfile);

    if (verbose) {
        printf("user = %s\n", name);
        gmp_printf("s (%d bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%d bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    mpz_clears(p, q, n, e, d, id, s, NULL);
    return 0;
}

// Printed text provided from reference file.
void print_synopsis(void) {
    printf("SYNOPSIS\n");
    printf("   Generates an RSA public/private key pair.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./keygen [-hv] [-b bits] -n pbfile -d pvfile\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -b bits         Minimum bits needed for public key n (default: 256).\n");
    printf("   -i confidence   Miller-Rabin iterations for testing primes (default: 50).\n");
    printf("   -n pbfile       Public key file (default: rsa.pub).\n");
    printf("   -d pvfile       Private key file (default: rsa.priv).\n");
    printf("   -s seed         Random seed for testing.\n");
}
