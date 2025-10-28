#include "memory_benchmarks.h"

uint64_t memory_acceses (uint64_t *array, uint64_t size, uint64_t index, uint64_t access_num){

    uint64_t sum = 0;
    for (int k = 0; k < access_num; k++){
            index = array[index];
            sum += index;
    }
    return sum;
}

uint64_t matrix_addition (uint64_t **matrix_a, uint64_t **matrix_b, uint64_t **result, uint64_t size){

    uint64_t sum = 0;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            result[i][j] = matrix_a[i][j] + matrix_b[i][j];
            sum += result[i][j];
        }
    }
    return sum;
}

uint64_t matrix_transpose (uint64_t **matrix_a, uint64_t **result, uint64_t size){

    uint64_t sum = 0;

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            result[j][i] = matrix_a[i][j];
            sum += result[i][j];
        }
    }
    return sum;
}

uint64_t matrix_transpose_multpl (uint64_t **matrix_a, uint64_t **result, uint64_t size){

    uint64_t sum = 0;
    uint64_t index;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            result[j][i] = matrix_a[i][j] * matrix_a[i][j];
            sum += result[i][j];
        }
    }
    return sum;
}

uint64_t vector_addition_int (uint64_t *vector_a, uint64_t *vector_b, uint64_t *result, uint64_t size){

    uint64_t sum = 0;
    for (int i = 0; i < size; i++){
        result[i] = vector_a[i] + vector_b[i];
        sum += result[i];
    }
    return sum;
}

uint64_t vector_multpl_int (uint64_t *vector_a, uint64_t *vector_b, uint64_t *result, uint64_t size){

    uint64_t sum = 0;
    for (int i = 0; i < size; i++){
        result[i] = vector_a[i] * vector_b[i];
        sum += result[i];
    }
    return sum;
}

uint64_t vector_multpl_add_int (uint64_t *vector_a, uint64_t *vector_b, uint64_t *vector_c, uint64_t *result, uint64_t size){

    uint64_t sum = 0;
    for (int i = 0; i < size; i++){
        result[i] = (vector_a[i] * vector_b[i]) + vector_c[i];
        sum += result[i];
    }
    return sum;
}

uint64_t rectifier_int (int64_t *vector_a, uint64_t *result, uint64_t size){

    uint64_t sum = 0;
    for (int i = 0; i < size; i++){
        result[i] = (vector_a[i] > 0) ? vector_a[i] : 0;
        sum += result[i];
    }
    return sum;
}

uint64_t vector_division_int (uint64_t *vector_a, uint64_t *vector_b, uint64_t *result, uint64_t size){

    uint64_t sum = 0;
    for (int i = 0; i < size; i++){
        result[i] = vector_a[i] / vector_b[i];
        sum += result[i];
    }
    return sum;

}

uint64_t matrix_multiply_basic (uint64_t **matrix_a, uint64_t **matrix_b, uint64_t **result, uint64_t size){

    uint64_t sum = 0;
    int jump;
    jump =  rand() % 2 + 45;
    for (int i = 0; i < size; i += jump) {
        for (int j = 0; j < size; j += jump) {
            for (int k = 0; k < size; k += jump) {
                result[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
            sum += result[i][j];
        }
    }
    return sum;

}

uint lidar_downsampling (uint64_t **matrix_a, uint64_t **result, uint64_t size, int reduction_factor){

    uint64_t sum = 0;
    for (int i = 0; i < (size / reduction_factor); i++){

        result[i][0] = matrix_a[reduction_factor*i][0];
        result[i][1] = matrix_a[reduction_factor*i][1];
        result[i][2] = matrix_a[reduction_factor*i][2];

        sum += result[i][0] + result[i][1] + result[i][2];
    }
    return sum;    
}

uint64_t lidar_filtering (uint64_t **matrix_a, uint64_t **result, uint64_t size){

    uint64_t sum = 0;
    int distance_threshold = 10;

    for (int i = 0; i < size; i++){
        
        if ((matrix_a[i][0] > distance_threshold) || (matrix_a[i][1] > distance_threshold) || (matrix_a[i][2] > distance_threshold)) {
            result[i][0] = matrix_a[i][0];
            result[i][1] = matrix_a[i][1];
            result[i][2] = matrix_a[i][2];
        }
        sum += result[i][0] + result[i][1] + result[i][2];
    }
    return sum;    
}

uint64_t scale_feature_vector (uint64_t *vector_a, uint64_t *result, uint64_t size, int scale_factor){

    uint64_t sum = 0;
    for (int i = 0; i < size; i++){
        result[i] = vector_a[i] * scale_factor;
        sum += result[i];
    }
    return sum;    
}