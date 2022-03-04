# Assignment 6: Public Key Cryptography
RSA asymmetric encryption is implemented in assignment 6. This process requires the use of three executables: **keygen**, **encrypt**, and **decrypt**. The **keygen** program generates a public, private key pair. The **encrpyt** program encrypts a file using the public key generated with **keygen**. The **decrypt** program decrypts the ciphertext generated with **encrypt** using the private key from **keygen**.

## Build
The commands **make** or **make all** builds all three executables. The command **make keygen** builds only **keygen**. The command **make encrypt** builds only **encrypt**. The command **make decrypt** builds only **decrypt**. And **make clean** cleans executables and object files.

## Run keygen
Generate a key pair through the options: ./keygen \[-hv\] \[-b bits\] -n pbfile -d pvfile
* -h              Display program help and usage.
* -v              Display verbose program output.
* -b bits         Minimum bits needed for public key n (default: 256).
* -i confidence   Miller-Rabin iterations for testing primes (default: 50).
* -n pbfile       Public key file (default: rsa.pub).
* -d pvfile       Private key file (default: rsa.priv).
* -s seed         Random seed for testing.

## Run encrypt
Encrypt a file through the options: ./encrypt \[-hv\] \[-i infile\] \[-o outfile\] -n pubkey 
* -h              Display program help and usage.
* -v              Display verbose program output.
* -i infile       Input file of data to encrypt (default: stdin).
* -o outfile      Output file for encrypted data (default: stdout).
* -n pbfile       Public key file (default: rsa.pub).

## Run decrypt
Decrypt a file through the options: ./decrypt \[-hv\] \[-i infile\] \[-o outfile\] -n privkey
* -h              Display program help and usage.
* -v              Display verbose program output.
* -i infile       Input file of data to decrypt (default: stdin).
* -o outfile      Output file for decrypted data (default: stdout).
* -n pbfile       Private key file (default: rsa.pub).

## Scan
**scan-build make** produces no errors or warnings

## Resources Used
I made use of Eugene's sections.


