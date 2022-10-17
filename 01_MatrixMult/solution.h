#pragma once

typedef struct _thread_param{
    int **a;
    int **b;
    int lenght;
    int **res;
    int up;
    int down;
}thread_param;

typedef struct _block{
    int **matrix;
    int offset1;
    int offset2;
}block;

int** make_random_matrix(int lenght);

int** allocate_matrix(int lenght);

void free_matrix(int **matrix, int lenght);

void print_matrix(int **matrix, int lenght);

int** naive_matrix_multiplication(int **a_matrix, int **b_matrix, int lenght, int n_threads);

int **one_block_multipy(block a_block, block b_block);

void one_block_pp(block res, int **a);

void *block_mult(void *arguments);

int** block_matrix_multiplication(int **a_matrix, int **b_matrix, int lenght, int n_threads);