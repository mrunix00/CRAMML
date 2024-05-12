#ifndef _CRAMML_LIB_
#define _CRAMML_LIB_

#include <stdint.h>
#include <stdlib.h>

#ifdef _USE_DOUBLE_PRECISION
typedef double number;
#else
typedef float number;
#endif

typedef struct {
	uint32_t columns;
	uint32_t rows;
} MatrixSize;

typedef struct {
	number **values;
	MatrixSize size;
} Matrix;

number n_at(const Matrix m, uint32_t row, uint32_t column);
Matrix row_at(const Matrix m, uint32_t row);
Matrix col_at(const Matrix m, uint32_t column);
void m_print(const Matrix m);

Matrix m_init(uint32_t columns, uint32_t rows, ...);
Matrix m_add(const Matrix m1, const Matrix m2);
Matrix m_mult(const Matrix m1, const Matrix m2);
Matrix m_smult(const Matrix m1, const Matrix m2);
Matrix sm_mult(number s, const Matrix m);
Matrix m_inv(const Matrix m);
Matrix m_diag(const Matrix m);
Matrix m_transpose(const Matrix m);
Matrix m_zeros(uint32_t rows, uint32_t columns);
Matrix m_ones(uint32_t rows, uint32_t columns);
Matrix m_eye(uint32_t rows, uint32_t columns);

void m_free(Matrix *m);

#endif
