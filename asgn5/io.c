#include "code.h"
#include "defines.h"
#include "io.h"

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

// Credit: Chrisitan
uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t bit_buf[BLOCK] = { 0 };
static int buf_index = 0;
static int buf_size = 0;
static int end = 0;
static int write_bit_index = 0;
static uint8_t code_buf[BLOCK] = { 0 };

// Credit: Christian
// Reads given number of bytes from infile into given buffer.
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int read_value = 0;
    int bytes = 0;
    while ((read_value = read(infile, buf + bytes, nbytes - bytes)) > 0) {
        bytes_read += read_value;
        bytes += read_value;
        if (bytes == nbytes) {
            return bytes;
        }
    }
    return bytes;
}

// Credit: Christian
// Writes given number of bytes from buffer into outfile.
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int write_value = 0;
    int bytes = 0;
    while ((write_value = write(outfile, buf + bytes, nbytes - bytes)) > 0) {
        bytes_written += write_value;
        bytes += write_value;
        if (bytes == nbytes) {
            return bytes;
        }
    }
    return bytes;
}

// Credit: Christian, Eugene
// Reads a single bit from given infile.
bool read_bit(int infile, uint8_t *bit) {
    if (buf_index == 0) {
        buf_size = read_bytes(infile, bit_buf, BLOCK);
        if (buf_size == 0) {
            return false;
        }
        if (buf_size < BLOCK) {
            end = buf_size * 8;
        }
    }
    *bit = (bit_buf[buf_index / 8] & (1 << (buf_index % 8))) >> (buf_index % 8);
    buf_index += 1;
    if (buf_index == BLOCK * 8) {
        buf_index = 0;
    }
    return buf_index != end;
}

// Credit: Eugene
// Writes the remaining bits from the code buffer.
void flush_codes(int outfile) {
    int bytes = 0;
    if (write_bit_index > 0) {
        write_bit_index = ceil((write_bit_index) / 8.0);
        bytes = write_bytes(outfile, code_buf, write_bit_index);
        for (int i = 0; i < bytes; i++) {
            code_buf[i] = 0;
        }
        write_bit_index = 0;
    }
}

// Credit: Eugene
// Writes the given code into a code buffer. If buffer is filled, buffer is written.
void write_code(int outfile, Code *c) {
    uint8_t bit = 0;
    for (uint32_t i = 0; i < code_size(c); i++) {
        bit = code_get_bit(c, i);
        if (bit == 1) {
            code_buf[write_bit_index / 8] |= (1 << (write_bit_index % 8));
        }
        write_bit_index += 1;
        if (write_bit_index == BLOCK * 8) {
            flush_codes(outfile);
        }
    }
}
