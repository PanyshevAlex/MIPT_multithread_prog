#include "solution.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BLOCK_SIZE 8

int** allocate_matrix(int lenght)
{
    int **matrix = (int**)malloc(lenght * sizeof(int*));
    for (int i = 0; i < lenght; i++)
        matrix[i] = (int*)malloc(lenght * sizeof(int));
    return matrix;
}

int** make_random_matrix(int lenght)
{
    int **matrix = allocate_matrix(lenght);

    for (int i = 0; i < lenght; i++)
        for (int j = 0; j < lenght; j++)
            matrix[i][j] = rand() % 10;
    return matrix;
}



void free_matrix(int **matrix, int lenght)
{
    for (int i = 0; i < lenght; i++)
        free(matrix[i]);
    free(matrix);
}

void print_matrix(int **matrix, int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
        for (int j = 0; j < lenght; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

void *naive_mult(void *arguments)
{

    thread_param *param = (thread_param*)arguments;

    for (int i = param->up; i < param->down; i++)
        for (int j = 0; j < param->lenght; j++)
        {
            param->res[i][j] = 0;
            for (int k = 0; k < param->lenght; k++)
                param->res[i][j] += param->a[i][k] * param->b[k][j];
        }
    free(param);
    pthread_exit(0);
}

int** naive_matrix_multiplication(int **a_matrix, int **b_matrix, int lenght, int n_threads)
{
    pthread_t tid[n_threads];
    int **res_matrix = allocate_matrix(lenght);

    for (int i = 0; i < n_threads; i++)
    {
        int up = i * lenght / n_threads;
        int down = (i+1) * lenght / n_threads;
        
        //thread_param param = {a_matrix, b_matrix, lenght, res_matrix, up, down};
        thread_param *param = (thread_param *)malloc(sizeof(thread_param));
        param->a = a_matrix;
        param->b = b_matrix;
        param->lenght = lenght;
        param->res = res_matrix;
        param->up = up;
        param->down = down;
        pthread_create(&tid[i], NULL, naive_mult, param);
    }
    for (int i = 0; i < n_threads; i++)
        pthread_join(tid[i], NULL);
    return res_matrix;
}

int **one_block_multipy(block a_block, block b_block)
{
    int **res_matrix = allocate_matrix(BLOCK_SIZE);

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            res_matrix[i][j] = 0;
            for (int k = 0; k < BLOCK_SIZE; k++)
                res_matrix[i][j] += a_block.matrix[a_block.offset1 + i][a_block.offset2 + k]*b_block.matrix[b_block.offset1 + k][b_block.offset2 + j];
        }
    }
    return res_matrix;
}

void one_block_pp(block res, int **a)
{
    for (int i = 0; i < BLOCK_SIZE; i++)
        for (int j = 0; j < BLOCK_SIZE; j++)
            res.matrix[res.offset1 + i][res.offset2 + j] += a[i][j];
}

void *block_mult(void *arguments)
{
    thread_param *param = (thread_param*)arguments;

    for (int i = param->up; i < param->down; i++)
    {
        for (int j = 0; j < param->lenght; j++)
        {
            //param->res[i][j] = 0;
            for (int p = 0; p < BLOCK_SIZE; p++)
                for (int q = 0; q < BLOCK_SIZE; q++)
                    param->res[i*BLOCK_SIZE + p][j*BLOCK_SIZE + q] = 0;

            for (int k = 0; k < param->lenght; k++)
            {
                //param->res[i][j] += param->a[i][k] * param->b[k][j];
                block a_block = {param->a, i*BLOCK_SIZE, k*BLOCK_SIZE};
                block b_block = {param->b, k*BLOCK_SIZE, j*BLOCK_SIZE};
                int **sum_tmp = one_block_multipy(a_block, b_block);
                block res_block = {param->res, i*BLOCK_SIZE, j*BLOCK_SIZE};
                one_block_pp(res_block, sum_tmp);
                free_matrix(sum_tmp, BLOCK_SIZE);
            }
        }
    }
    free(param);
    pthread_exit(0);
}

int** block_matrix_multiplication(int **a_matrix, int **b_matrix, int lenght, int n_threads)
{
    pthread_t tid[n_threads];
    int **res_matrix = allocate_matrix(lenght);
    for (int i = 0; i < n_threads; i++)
    {
        int up = i * lenght / (n_threads * BLOCK_SIZE);
        int down = (i+1) * lenght / (n_threads * BLOCK_SIZE);
        thread_param *param = (thread_param *)malloc(sizeof(thread_param));
        param->a = a_matrix;
        param->b = b_matrix;
        param->lenght = lenght / BLOCK_SIZE;
        param->res = res_matrix;
        param->up = up;
        param->down = down;
        pthread_create(&tid[i], NULL, block_mult, param);
    }
    for (int i = 0; i < n_threads; i++)
        pthread_join(tid[i], NULL);
    return res_matrix;
}