#include "hamming.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //need for strdup
#include <sys/stat.h> //used to set permision
#include <sys/types.h>
#include <unistd.h> //for getop()

#define OPTIONS "hi:o:v" //options for input
uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}
uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}
uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

int main(int argc, char **argv) {
    int32_t opt = 0;
    FILE *output = stdout;
    FILE *input = stdin;
    bool verbose = false;
    double bytesread = 0;
    double numerror = 0;
    uint32_t corrected = 0;

    int c;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //program argument parser.
        switch (opt) {
        case 'h':
            fprintf(stderr, "SYNOPSIS\n"
                            "  A Hamming(8, 4) systematic code decoder.\n"
                            "\n"
                            "USAGE\n"
                            "  ./test/decode [-h] [-v] [-i infile] [-o outfile]\n"
                            "\n"
                            "OPTIONS\n"
                            "  -h             Program usage and help.\n"
                            "  -v             Print statistics of decoding to stderr.\n"
                            "  -i infile      Input data to decode.\n"
                            "  -o outfile     Output of decoded data.\n");
            exit(1);
            break;
        case 'i': input = fopen(optarg, "rb"); break;
        case 'o': output = fopen(optarg, "wb"); break;
        case 'v': verbose = true; break;
        }
    }

    struct stat statbuff;
    fstat(fileno(input), &statbuff);
    fchmod(fileno(output), statbuff.st_mode);
    //create Henorator Matrix H
    BitMatrix *H = bm_create(8, 4);
    //
    bm_set_bit(H, 0, 1);
    bm_set_bit(H, 0, 2);
    bm_set_bit(H, 0, 3);
    //
    bm_set_bit(H, 1, 0);
    bm_set_bit(H, 1, 2);
    bm_set_bit(H, 1, 3);
    //
    bm_set_bit(H, 2, 0);
    bm_set_bit(H, 2, 1);
    bm_set_bit(H, 2, 3);
    //
    bm_set_bit(H, 3, 0);
    bm_set_bit(H, 3, 1);
    bm_set_bit(H, 3, 2);
    //
    bm_set_bit(H, 4, 0);
    //
    bm_set_bit(H, 5, 1);
    bm_set_bit(H, 6, 2);
    bm_set_bit(H, 7, 3);

    HAM_STATUS retvalue;
    while ((c = fgetc(input)) != EOF) {
        bytesread += 2;
        uint8_t msg1 = 0;
        uint8_t msg2 = 0;
        retvalue = decode(H, c, &msg1);
        //stats collector
        if (retvalue == HAM_ERR) {
            numerror += 1;
        } else if (retvalue == HAM_CORRECT) {
            corrected += 1;
        }
        //

        c = fgetc(input);
        retvalue = decode(H, c, &msg2);
        //stats collector
        if (retvalue == HAM_ERR) {
            numerror += 1;
        } else if (retvalue == HAM_CORRECT) {
            corrected += 1;
        }
        //
        fputc(pack_byte(msg2, msg1), output);
    }
    if (verbose) {
        //print data
        printf("Total bytes processed: %u\n", (uint32_t) bytesread);
        printf("Uncorrected errors: %u\n", (uint32_t) numerror);
        printf("Corrected errors: %u\n", corrected);
        printf("Error rate: %1.6lf\n", (bytesread != 0 ? (numerror / bytesread) : 0));
    }
    //close program
    bm_delete(&H);
    fclose(output);
    fclose(input);
    return 0;
}
