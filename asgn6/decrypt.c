#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"

#include <gmp.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define OPTIONS "i:o:n:vh"

void print_synopsis(void);

int main(int argc, char **argv) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *priv_key;
    char *key_name = "rsa.priv";
    int opt = 0;
    bool verbose = false;
    bool help = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': key_name = optarg; break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        }
    }

    // These three conditionalds are help and error handling.
    if (optopt || help) {
        print_synopsis();
        if (infile != NULL) {
            fclose(infile);
        }
        fclose(outfile);
        exit(0);
    }

    if (infile == NULL) {
        printf("Infile failed to open.\n");
        fclose(outfile);
        exit(1);
    }

    if ((priv_key = fopen(key_name, "r")) == NULL) {
        printf("Private key file failed to open.\n");
        fclose(infile);
        fclose(outfile);
        exit(1);
    }

    mpz_t n, d;
    mpz_inits(n, d, NULL);

    rsa_read_priv(n, d, priv_key);

    if (verbose) {
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    rsa_decrypt_file(infile, outfile, n, d);

    fclose(infile);
    fclose(outfile);
    fclose(priv_key);
    mpz_clears(n, d, NULL);
    return 0;
}

// Printed text provided from reference file.
void print_synopsis(void) {
    printf("SYNOPSIS\n");
    printf("   Decrypts data using RSA encryption.\n");
    printf("   Encrypted data is encrypted by the encrypt program.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./decrypt [-hv] [-i infile] [-o outfile] -n privkey\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -i infile       Input file of data to decrypt (default: stdin).\n");
    printf("   -o outfile      Output file for decrypted data (default: stdout).\n");
    printf("   -n pbfile       Private key file (default: rsa.pub).\n");
}
