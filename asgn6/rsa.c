#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"
#include <stdlib.h>

// Creates a new RSA public key. p and q are primes; n is their product.
// e is the public exponent.
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t min_bits = nbits / 4;
    uint64_t num = (nbits * 3) / 4;

    mpz_t bits, bit_num, totient, gcd_value;
    mpz_inits(bits, bit_num, totient, gcd_value, NULL);
    mpz_set_ui(bit_num, num);

    do {
        mpz_urandomm(bits, state, bit_num);
    } while (mpz_cmp_ui(bits, min_bits) < 0);

    uint64_t total_bits = mpz_get_ui(bits);
    // 1 is added to each prime's total bits in order to make sure
    // n has at least nbits.
    make_prime(p, total_bits + 1, iters);
    make_prime(q, nbits - total_bits + 1, iters);

    // Totient(n) = (p - 1)(q - 1).
    mpz_mul(totient, p, q);
    mpz_set(n, totient);
    mpz_sub(totient, totient, p);
    mpz_sub(totient, totient, q);
    mpz_add_ui(totient, totient, 1);

    do {
        do {
            mpz_urandomb(e, state, nbits);
        } while ((uint64_t) mpz_sizeinbase(e, 2) < nbits);
        gcd(gcd_value, e, totient);
    } while (mpz_cmp_ui(gcd_value, 1) != 0);

    mpz_clears(bits, bit_num, totient, gcd_value, NULL);
}

// Writes a public key to pbfile.
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
}

// Reads a public key from pbfile.
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
}

// Creates a new private key d from primes p and q and public exponent e.
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t totient;
    mpz_init(totient);
    mpz_mul(totient, p, q);
    mpz_sub(totient, totient, p);
    mpz_sub(totient, totient, q);
    mpz_add_ui(totient, totient, 1);
    mod_inverse(d, e, totient);
    mpz_clear(totient);
}

// Writes a private key to pvfile.
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

// Reads a private key from pvfile.
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

// Encrpyts message m into ciphertext c.
// Formula: E(m) = c = m^e (mod n).
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}

// Credit: Eugene
// Encrypts infile and writes contents to outfile.
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    uint64_t block = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *buf = (uint8_t *) calloc(block, sizeof(uint8_t));
    buf[0] = 0xFF;
    uint64_t j = 0;
    mpz_t m, c;
    mpz_inits(m, c, NULL);

    while ((j = fread(buf + 1, sizeof(uint8_t), block - 1, infile)) != 0) {
        mpz_import(m, j + 1, 1, 1, 1, 0, buf);
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    free(buf);
    mpz_clears(m, c, NULL);
}

// Performs RSA decryption on ciphertext c.
// Formula: D(c) = m = c^d (mode n).
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}

// Decrypts infile and writes plaintext to outfile.
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    uint32_t block = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *buf = (uint8_t *) calloc(block, sizeof(uint8_t));
    uint64_t j = 0;
    mpz_t c, m;
    mpz_inits(c, m, NULL);

    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {
        rsa_decrypt(m, c, d, n);
        mpz_export(buf, &j, 1, 1, 1, 0, m);
        fwrite(buf + 1, sizeof(uint8_t), j - 1, outfile);
    }
    free(buf);
    mpz_clears(c, m, NULL);
}

// Produces an RSA signature s.
// Formula: S(m) = s = m^d (mod n).
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
}

// Verifies RSA signature s.
// Let t = V(s) = s^e. Signature is verified if t = m.
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}
