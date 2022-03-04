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
    FILE *pub_key;
    char *key_name = "rsa.pub";
    char name[50];
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

    // These next three conditionals are help and error handling.
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

    if ((pub_key = fopen(key_name, "r")) == NULL) {
        printf("Public key file failed to open.\n");
        fclose(infile);
        fclose(outfile);
        exit(1);
    }

    mpz_t n, e, s, id;
    mpz_inits(n, e, s, id, NULL);

    rsa_read_pub(n, e, s, name, pub_key);

    if (verbose) {
        printf("user = %s\n", name);
        gmp_printf("s (%d bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%d bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
    }

    mpz_set_str(id, name, 62);

    if (rsa_verify(id, s, e, n) == false) {
        printf("Signature couldn't be verified.\n");
        fclose(infile);
        fclose(outfile);
        fclose(pub_key);
        mpz_clears(n, e, s, id, NULL);
        exit(1);
    }

    rsa_encrypt_file(infile, outfile, n, e);

    fclose(infile);
    fclose(outfile);
    fclose(pub_key);
    mpz_clears(n, e, s, id, NULL);
    return 0;
}

// Printed text provided from reference file.
void print_synopsis(void) {
    printf("SYNOPSIS\n");
    printf("   Encrypts data using RSA encryption.\n");
    printf("   Encrypted data is decrypted bt the decrypt program.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
    printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
    printf("   -n pbfile       Public key file (default: rsa.pub).\n");
}
