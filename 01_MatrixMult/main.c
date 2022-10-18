//
// main.c
// 01_MatrixMult
//
// Created by Panyshev Alexander on 14.10.2022.
//


#include "solution.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
int main(int argc, char **argv)
{
	if (argc != 3)
    {
        printf("Usage: %s matrix_lenght threads_number", argv[0]);
        return 1;
    }

    // int lenght = atoi(argv[1]);
    // int num_threads = atoi(argv[2]);
    char *pend;
    int lenght = (int)strtol(argv[1], &pend, 10);
    if (lenght == 0)
    {
        perror("Incorrect matrix_lenght");
        return 1;
    }
    if (*pend != '\0')
    {
        printf("Invalid matrix_lenght\n");
        return 1;
    }
    int num_threads = (int)strtol(argv[2], &pend, 10);
    if (num_threads == 0)
    {
        perror("Incorrect threads_number");
        return 1;
    }
    if (*pend != '\0')
    {
        printf("Invalid threads_number\n");
        return 1;
    }
    srand(time(NULL));
    int **a = make_random_matrix(lenght);
    int **b = make_random_matrix(lenght);
    printf("matrix a: \n");
    print_matrix(a, lenght);
    printf("--------------------------------------------\n");
    printf("matrix b: \n");
    print_matrix(b, lenght);
    printf("--------------------------------------------\n");
    int **c = naive_matrix_multiplication(a, b, lenght, num_threads);
    printf("Naive matrix mult\n");
    print_matrix(c, lenght);
    printf("--------------------------------------------\n");
    int **d = block_matrix_multiplication(a, b, lenght, num_threads);
    printf("Block matrix mult\n");
    print_matrix(d, lenght);
    printf("--------------------------------------------\n");

    free_matrix(a, lenght);
    free_matrix(b, lenght);
    free_matrix(c, lenght);
    free_matrix(d, lenght);

    // время умножения матриц от размера матриц для наивной однопоточной реализации
    // Графики
    // FILE *file;
    // file = fopen("1.txt", "w");
    // struct timespec start, end;
    // int lenght = 500;
    // int **a = make_random_matrix(lenght);
    // int **b = make_random_matrix(lenght);
    // for (int i = 1; i <= 8; i++)
    // {
    //     clock_gettime(CLOCK_REALTIME, &start);
    //     int **c = block_matrix_multiplication(a, b, lenght, i);
    //     clock_gettime(CLOCK_REALTIME, &end);
    //     printf("thread %d finished\n", i);
    //     free_matrix(c, lenght);
    //     double conseq_time = end.tv_sec - start.tv_sec;
    //     conseq_time += (end.tv_nsec - start.tv_nsec) / (double)1e9;
    //     fprintf(file, "%d %f\n", i, conseq_time);
    // }
    // fclose(file);

	return 0;
}