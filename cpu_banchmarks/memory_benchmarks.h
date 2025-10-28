#ifndef _MEMORY_BENCHMARKS_H_
#define _MEMORY_BENCHMARKS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>

#define ACCESS_NUM 1000000
#define ACCESS_NUM_2 2000000
#define VECTOR_SIZE_ACCSS 400000000 
#define VECTOR_SIZE_ALL_OPS 100000000
#define VECTOR_SIZE_MULTPL_ADD 80000000
#define MATRIX_SIZE_SUM 10000
#define MATRIX_SIZE_TRANS 10000
#define MATRIX_SIZE_MULTPL 15000
#define POINTCLOUD_SIZE 20000000
#define DOWNSAMPLING_FACTOR_2 2
#define DOWNSAMPLING_FACTOR_4 4
#define DOWNSAMPLING_FACTOR_10 10
#define SCALE_FACTOR 3

#define ACCESS_NUM_2_L2 2000000 * 5
#define VECTOR_SIZE_ACCSS_L2 200000 
#define VECTOR_SIZE_ALL_OPS_L2 200000
#define VECTOR_SIZE_MULTPL_ADD_L2 200000
#define MATRIX_SIZE_SUM_L2 500
#define MATRIX_SIZE_TRANS_L2 500
#define MATRIX_SIZE_MULTPL_L2 500
#define POINTCLOUD_SIZE_L2 80000

#define ACCESS_NUM_2_L1 20000000
#define VECTOR_SIZE_ACCSS_L1 8000 
#define VECTOR_SIZE_ALL_OPS_L1 8000
#define VECTOR_SIZE_MULTPL_ADD_L1 8000
#define MATRIX_SIZE_SUM_L1 90
#define MATRIX_SIZE_TRANS_L1 90
#define MATRIX_SIZE_MULTPL_L1 90
#define POINTCLOUD_SIZE_L1 2500

volatile uint64_t memory_acceses (uint64_t *array, uint64_t size, uint64_t index, uint64_t access_num);

volatile uint64_t matrix_addition (uint64_t **matrix_a, uint64_t **matrix_b, uint64_t **result, uint64_t size);

volatile uint64_t matrix_transpose (uint64_t **matrix_a, uint64_t **result, uint64_t size);

volatile uint64_t matrix_transpose_multpl (uint64_t **matrix_a, uint64_t **result, uint64_t size);

volatile uint64_t vector_addition_int (uint64_t *vector_a, uint64_t *vector_b, uint64_t *result, uint64_t size);

volatile float vector_addition_float (float *vector_a, float *vector_b, float *result, float size);

volatile uint64_t vector_multpl_int (uint64_t *vector_a, uint64_t *vector_b, uint64_t *result, uint64_t size);

volatile uint64_t vector_multpl_add_int (uint64_t *vector_a, uint64_t *vector_b, uint64_t *vector_c, uint64_t *result, uint64_t size);

volatile uint64_t rectifier_int (int64_t *vector_a, uint64_t *result, uint64_t size);

volatile uint64_t vector_division_int (uint64_t *vector_a, uint64_t *vector_b, uint64_t *result, uint64_t size);

volatile uint64_t matrix_multiply_basic (uint64_t **matrix_a, uint64_t **matrix_b, uint64_t **result, uint64_t size);

volatile uint64_t matrix_multiply_optimized (uint64_t **matrix_a, uint64_t **matrix_b, uint64_t **result, uint64_t size);

volatile uint lidar_downsampling (uint64_t **matrix_a, uint64_t **result, uint64_t size, int reduction_factor);

volatile uint64_t lidar_filtering (uint64_t **matrix_a, uint64_t **result, uint64_t size);

volatile uint64_t scale_feature_vector (uint64_t *vector_a, uint64_t *result, uint64_t size, int scale_factor);

#endif // _MEMORY_BENCHMARKS_H_