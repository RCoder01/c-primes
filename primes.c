#include <stdio.h> // printf
#include <stdint.h> // uint8_t, uint64_t, size_t, etc.
#include <stdlib.h> // malloc, free
#include <math.h> // sqrt

static size_t SIZE = 1000000000;

static size_t END_BYTE = 0;

// Index is the position in the bit array, value is the number it represents
// Values are [3, 5, 7, 9, ...], indices are [0, 1, 2, 3, ...]
// Get value from index
uint64_t from_index(size_t index) {
    return (index * 2) + 3;
}

// Get index from value
size_t to_index(uint64_t value) {
    return (value - 3) / 2;
}

size_t next_index(const uint8_t* sieve, size_t start) {
    // Iterate through bits and return index once bit is 1
    while (start / 8 < END_BYTE) {
        if (sieve[start / 8] & (1 << start % 8)) {
            return start;
        }
        start += 1;
    }
    return -1;
}

// Number or bits in a byte
static uint8_t BIT_COUNTS[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

uint8_t bit_count(uint8_t byte) {
    // Just index into the hardcoded list
    return BIT_COUNTS[byte];
}

int main() {
    END_BYTE = (size_t) (sqrt((double) SIZE) / 8 / 2 + 1);

    uint8_t* sieve = malloc(SIZE/8/2);
    // Initialize to all 1s
    for (size_t byte = 0; byte < SIZE/8/2; byte++) {
        sieve[byte] = 255;
    }
    // Start from index 0 (which is the value 3)
    size_t index = 0;
    while (index != -1) {
        uint64_t stride = from_index(index);
        // equivalent to `to_index(from_index(index) ** 2)`
        size_t start = 2 * index * index + 6 * index + 3;
        size_t replace_index = 0;
        // Iterate through multiples of the prime and set their bits to 0
        for (replace_index = start; replace_index < (SIZE / 2); replace_index += stride) {
            sieve[replace_index / 8] &= ~((uint8_t) 1 << (replace_index % 8));
        }
        index = next_index(sieve, index + 1);
    }
    // Count the number of bits left in the sieve
    uint64_t count = 1;
    for (size_t byte = 0; byte < SIZE/8/2; byte++) {
        count += bit_count(sieve[byte]);
    }
    printf("%ld primes less than %ld\n", count, SIZE);
    free(sieve);
}
