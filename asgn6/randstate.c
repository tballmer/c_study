#include "randstate.h"

gmp_randstate_t state;

// Initializes the global random state with a Mersenne Twister algorithm.
void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

// Clears and frees memory used by state.
void randstate_clear(void) {
    gmp_randclear(state);
}
