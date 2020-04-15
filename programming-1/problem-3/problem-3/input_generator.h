#pragma once

#include <stdlib.h>

#include "matrix.h"

matrix *generate_matrix(size_t max_size, int upper_limit);
matrix *generate_matrix_array(size_t max_size, int upper_limit, size_t num);

int *generate_numbers_array(int upper_limit, size_t num);
