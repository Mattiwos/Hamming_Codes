#include "bm.h"

#include "bv.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
} BitMatrix;

BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) calloc(1, sizeof(BitMatrix));
    if (!m) {
        m = NULL;
        return NULL;
    }
    m->rows = rows;
    m->cols = cols;
    m->vector = bv_create(rows * cols);
    assert(m->vector);
    return m;
}

void bm_delete(BitMatrix **m) {
    assert(*m);
    bv_delete(&(*m)->vector);
    free(*m);
    *m = NULL;
    return;
}

uint32_t bm_rows(BitMatrix *m) {
    assert(m);
    return m->rows;
}

uint32_t bm_cols(BitMatrix *m) {
    assert(m);
    return m->cols;
}

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    assert(m);
    bv_set_bit(m->vector, (r * m->cols) + c);
    return;
}

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    assert(m);
    bv_clr_bit(m->vector, (r * m->cols) + c);
    return;
}

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    assert(m);
    return bv_get_bit(m->vector, (r * m->cols) + c);
}

BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    BitMatrix *c = bm_create(1, length);
    for (uint32_t i = 0; i < length; i++) {
        if (byte & (1 << i)) {
            bm_set_bit(c, 0, i);
        } else {
            bm_clr_bit(c, 0, i);
        }
    }
    return c;
}

uint8_t bm_to_data(BitMatrix *m) {
    assert(m);
    uint8_t c = 0;
    for (uint32_t i = 0; i < m->cols; i++) {
        c |= (bm_get_bit(m, 0, i)) << i;
    }
    return c;
}

BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    BitMatrix *C = bm_create(bm_rows(A), bm_cols(B));
    for (uint32_t i = 0; i < A->rows; i++) { //rows in a
        for (uint32_t j = 0; j < B->cols; j++) { //cols in B
            uint8_t val = 0;
            for (uint32_t k = 0; k < A->cols; k++) {
                val ^= bm_get_bit(A, i, k) & bm_get_bit(B, k, j);
            }
            if (val % 2) {
                bm_set_bit(C, i, j);
            }
        }
    }
    return C;
}

void bm_print(BitMatrix *m) {
    printf("Bits: \n");
    printf("Rows %u and cols %u \n", m->rows, m->cols);
    for (uint32_t i = 0; i < m->rows; i++) {
        for (uint32_t j = 0; j < m->cols; j++) {
            printf("%u ", bm_get_bit(m, i, j));
        }
        printf("\n");
    }
    return;
}
