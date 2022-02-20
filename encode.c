#include "bm.h"
#include "hamming.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //need for strdup
#include <sys/stat.h> //used to set permision
#include <sys/types.h>
#include <unistd.h> //for getop()

#define OPTIONS "hi:o:" //options for input

uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}
uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}
int main(int argc, char **argv) {
    int32_t opt = 0;
    FILE *output = stdout;
    FILE *input = stdin;
    int c;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //program argument parser.
        switch (opt) {
        case 'h':
            //print message
            fprintf(stderr, "SYNOPSIS\n"
                            "  A Hamming(8, 4) systematic code generator.\n"
                            "\n"
                            "USAGE\n"
                            "  ./test/encode [-h] [-i infile] [-o outfile]\n"
                            "\n"
                            "OPTIONS\n"
                            "  -h             Program usage and help.\n"
                            "  -i infile      Input data to encode.\n"
                            "  -o outfile     Output of encoded data.\n");
            exit(1);
            break;
        case 'i': input = fopen(optarg, "rb"); break;
        case 'o': output = fopen(optarg, "wb"); break;
        }
    }
    struct stat statbuff;
    fstat(fileno(input), &statbuff);
    fchmod(fileno(output), statbuff.st_mode);

    //create Genorator Matrix G
    BitMatrix *G = bm_create(4, 8);
    //
    bm_set_bit(G, 0, 0);
    bm_set_bit(G, 0, 5);
    bm_set_bit(G, 0, 6);
    bm_set_bit(G, 0, 7);
    //
    bm_set_bit(G, 1, 1);
    bm_set_bit(G, 1, 4);
    bm_set_bit(G, 1, 6);
    bm_set_bit(G, 1, 7);
    //
    bm_set_bit(G, 2, 2);
    bm_set_bit(G, 2, 4);
    bm_set_bit(G, 2, 5);
    bm_set_bit(G, 2, 7);
    //
    bm_set_bit(G, 3, 3);
    bm_set_bit(G, 3, 4);
    bm_set_bit(G, 3, 5);
    bm_set_bit(G, 3, 6);
    //
    //
    while ((c = fgetc(input)) != EOF) {
        uint8_t msg1 = lower_nibble(c);
        uint8_t msg2 = upper_nibble(c);
        uint8_t code1 = encode(G, msg1);
        fputc(code1, output);
        uint8_t code2 = encode(G, msg2);
        fputc(code2, output);
    }

    //close program
    bm_delete(&G);
    fclose(output);
    fclose(input);
    return 0;
}
