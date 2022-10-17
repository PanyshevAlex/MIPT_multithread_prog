//
// main.c
// ff
//
// Created by Panyshev Alexander on 14.10.2022.
//


#include "solution.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	if (argc != 3)
    {
        printf("Usage: %s matrix_lenght threads_number", argv[0]);
        return 1;
    }

    int lenght = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    srand(time(NULL));
    int **a = make_random_matrix(lenght);
    int **b = make_random_matrix(lenght);
    // printf("matrix a: \n");
    // print_matrix(a, lenght);
    // printf("--------------------------------------------\n");
    // printf("matrix b: \n");
    // print_matrix(b, lenght);
    // printf("--------------------------------------------\n");
    int **c = naive_matrix_multiplication(a, b, lenght, num_threads);
    print_matrix(c, lenght);
    printf("--------------------------------------------\n");
    int **d = block_matrix_multiplication(a, b, lenght, num_threads);
    print_matrix(d, lenght);
    printf("--------------------------------------------\n");

    free_matrix(a, lenght);
    free_matrix(b, lenght);
    free_matrix(c, lenght);
    free_matrix(d, lenght);

    int **t1 = make_random_matrix(16);
    printf("matrix t1: \n");
    print_matrix(t1, 16);
    printf("--------------------------------------------\n");
    block t1_block = {t1, 8, 0};
    block t2_block = {t1, 8, 8};
    int **tres = one_block_multipy(t1_block, t2_block);
    printf("matrix tres: \n");
    print_matrix(tres, 8);
    printf("--------------------------------------------\n");
	return 0;
}