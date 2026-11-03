/*
* File:   main.c
 * Author: gerstl
 *
 * (c) Copyright, 2025 all rights reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include "memoryTest.h"
#include <locale.h> /* for comma separated numbers */
/*
 *
 */
int main(int argc, char** argv) {
    setlocale(LC_NUMERIC,"");
    int return_value = EXIT_SUCCESS;
    if (0 != memoryTestOne()) {
        fprintf(stderr,"Failure in memory test one");
        return_value = EXIT_FAILURE;
    }
    if (0 != memoryTestTwo()) {
        fprintf(stderr,"Failure in memory test two");
        return_value = EXIT_FAILURE;
    }
    if (0 != memoryTestThree()) {
        fprintf(stderr,"Failure in memory test three");
        return_value = EXIT_FAILURE;
    }
    if (EXIT_SUCCESS==return_value) {
        fprintf(stdout,"Tests Succeeded");
    }
    fflush(stdout);
    fflush(stderr);
    return (return_value);
}

