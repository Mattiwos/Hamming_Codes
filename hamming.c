#include "hamming.h"

#include <stdio.h>

const int8_t lookup[16] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR, HAM_ERR, 2,
    HAM_ERR, 1, 0, HAM_ERR };

uint8_t encode(BitMatrix *G, uint8_t msg) {
    BitMatrix *m = bm_from_data(msg, 4);
    BitMatrix *code = bm_multiply(m, G);
    uint8_t f = bm_to_data(code);
    bm_delete(&m);
    bm_delete(&code);
    return f;
}

HAM_STATUS decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    BitMatrix *c = bm_from_data(code, 8);
    //bm_print(c);
    BitMatrix *err = bm_multiply(c, Ht);
    uint8_t errormsg = bm_to_data(err);
    //no error msg
    if (errormsg == 0) {
        *msg = bm_to_data(c);
        bm_delete(&c);
        bm_delete(&err);
        return HAM_OK;
    }
    //lookup table
    else {
        //too many erros
        if (lookup[errormsg] == HAM_ERR) {
            *msg = bm_to_data(c);
            bm_delete(&c);
            bm_delete(&err);
            return HAM_ERR;
        }
        //able to correct msg
        else {
            if (bm_get_bit(c, 1, lookup[errormsg]) == 0) {
                bm_set_bit(c, 1, lookup[errormsg]);
            } else {
                bm_clr_bit(c, 1, lookup[errormsg]);
            }
            *msg = bm_to_data(c);
            bm_delete(&c);
            bm_delete(&err);
            return HAM_CORRECT;
        }
    }
    return HAM_OK;
}
