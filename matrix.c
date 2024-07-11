#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"

static void
panic(char *msg)
{
	fprintf(stderr, "[-] Fatal Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

static void *
safe_malloc(size_t size)
{
	void *buff = malloc(size);
	if (buff == NULL)
		panic("Memory allocation failed!");
	return buff;
}

number
n_at(const Matrix m, uint32_t row, uint32_t column)
{
	return m.values[row][column];
}

Matrix
row_at(const Matrix m, uint32_t row)
{
	return m_init(m.size.columns, 1, m.values[0]);
}

Matrix
col_at(const Matrix m, uint32_t column)
{
	Matrix col;
	uint32_t i;
	col.size.columns = 1;
	col.size.rows = m.size.rows;
	for (i = 0; i < col.size.rows; i++) {
		col.values[i] = safe_malloc(sizeof(number *));
		*col.values[i] = m.values[i][0];
	}
	return col;
}

void
m_print(const Matrix m)
{
	uint32_t i, j;
	for (i = 0; i < m.size.rows; i++) {
		printf("|");
		for (j = 0; j < m.size.columns; j++) {
			printf(" %f ", m.values[i][j]);
		}
		printf("|\n");
	}
}

Matrix
m_init(uint32_t columns, uint32_t rows, ...)
{
	Matrix m;
	uint32_t i, j;
	va_list args;
	size_t size;
	number *row;

	m.size.rows = rows;
	m.size.columns = columns;
	m.values = (number **)safe_malloc(rows * sizeof(number *));

	va_start(args, rows);
	for (i = 0; i < rows; i++) {
		size = columns * sizeof(number);
		m.values[i] = (number *)safe_malloc(size);
		row = va_arg(args, number *);
		for (j = 0; j < columns; j++)
			m.values[i][j] = row[j];
	}

	return m;
}

Matrix
m_add(const Matrix m1, const Matrix m2)
{
	Matrix result;
	uint32_t i, j;

	assert(m1.size.columns == m2.size.rows && m1.size.rows == m2.size.rows);

	result.size.rows = m1.size.rows;
	result.size.columns = m1.size.columns;
	result.values = (number **)safe_malloc(
	    result.size.rows * sizeof(number *));

	for (i = 0; i < result.size.rows; i++) {
		result.values[i] = (number *)safe_malloc(
		    result.size.columns * sizeof(number *));
		for (j = 0; j < result.size.columns; j++)
			result.values[i][j] = m1.values[i][j] + m2.values[i][j];
	}
	return result;
}

Matrix
m_mult(const Matrix m1, const Matrix m2)
{
	Matrix result = m_zeros(m1.size.rows, m1.size.rows);
	uint32_t i, j, k, sum;

	for (i = 0; i < result.size.rows; i++) {
		for (k = 0; k < result.size.columns; k++) {
			for (j = 0; j < result.size.rows; j++)
				result.values[i][j] += m1.values[i][k] *
				    m2.values[k][j];
		}
	}

	return result;
}

Matrix
m_smult(const Matrix m1, const Matrix m2)
{
	Matrix result;
	uint32_t i, j;

	assert(
	    m1.size.columns == m2.size.columns && m1.size.rows == m2.size.rows);

	result.size.rows = m1.size.rows;
	result.size.columns = m1.size.columns;
	result.values = (number **)safe_malloc(
	    result.size.rows * sizeof(number *));

	for (i = 0; i < result.size.rows; i++) {
		result.values[i] = (number *)malloc(
		    result.size.columns * sizeof(number));
		for (j = 0; j < result.size.columns; i++)
			result.values[i][j] = m1.values[i][j] * m2.values[i][j];
	}

	return result;
}

Matrix
sm_mult(number s, const Matrix m)
{
	Matrix result;
	uint32_t i, j;

	result.size = m.size;
	result.values = (number **)safe_malloc(
	    result.size.rows * sizeof(number *));

	for (i = 0; i < result.size.rows; i++) {
		result.values[i] = (number *)safe_malloc(
		    result.size.columns * sizeof(number));
		for (j = 0; j < result.size.columns; j++)
			result.values[i][j] = m.values[i][j] * s;
	}

	return result;
}

Matrix
m_inv(const Matrix m)
{
	/* This is stupidly incorrect, idk where did I come up with this fucking
	 * idea*/
	return m_mult(m, m_eye(m.size.rows, m.size.columns));
}

Matrix
m_diag(const Matrix m)
{
	Matrix diag;
	uint32_t i;
	diag.size.columns = 1;
	diag.size.rows = m.size.rows < m.size.columns ? m.size.rows :
							m.size.columns;
	diag.values = (number **)safe_malloc(diag.size.rows * sizeof(number *));

	for (i = 0; i < diag.size.rows; i++) {
		diag.values[i] = (number *)safe_malloc(sizeof(number));
		diag.values[i][0] = m.values[i][i];
	}

	return diag;
}

Matrix
m_transpose(const Matrix m)
{
	Matrix result;
	uint32_t i, j;

	result.size.rows = m.size.columns;
	result.size.columns = m.size.rows;
	result.values = (number **)safe_malloc(
	    result.size.rows * sizeof(number *));

	for (i = 0; i < result.size.rows; i++) {
		result.values[i] = (number *)safe_malloc(
		    result.size.columns * sizeof(number));
		for (j = 0; j < result.size.columns; j++)
			result.values[i][j] = m.values[j][i];
	}

	return result;
}

Matrix
m_zeros(uint32_t rows, uint32_t columns)
{
	Matrix m;
	uint32_t i, j;

	m.size.rows = rows;
	m.size.columns = columns;
	m.values = (number **)safe_malloc(rows * sizeof(number *));

	for (i = 0; i < rows; i++) {
		m.values[i] = (number *)safe_malloc(columns * sizeof(number));
		for (j = 0; j < columns; j++)
			m.values[i][j] = 1;
	}

	return m;
}

Matrix
m_ones(uint32_t rows, uint32_t columns)
{
	Matrix m;
	uint32_t i, j;

	m.size.rows = rows;
	m.size.columns = columns;
	m.values = (number **)safe_malloc(rows * sizeof(number *));

	for (i = 0; i < rows; i++) {
		m.values[i] = (number *)safe_malloc(columns * sizeof(number));
		for (j = 0; j < columns; j++)
			m.values[i][j] = 1;
	}

	return m;
}

Matrix
m_eye(uint32_t rows, uint32_t columns)
{
	Matrix m;
	uint32_t i, j;

	m.size.rows = rows;
	m.size.columns = columns;
	m.values = (number **)safe_malloc(rows * sizeof(number *));

	for (i = 0; i < rows; i++) {
		m.values[i] = (number *)safe_malloc(columns * sizeof(number));
		for (j = 0; j < columns; j++)
			m.values[i][j] = i == j ? 1 : 0;
	}

	return m;
}

void
m_free(Matrix *m)
{
	uint32_t i;
	for (i = 0; i < m->size.rows; i++)
		free(m->values[i]);
	free(m->values);
}
