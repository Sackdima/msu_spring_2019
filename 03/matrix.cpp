#include <cstdlib>
#include <stdexcept>

#include "matrix.h"

Matrix::Row::Row(int *row, size_t cols)
		: columns(cols), row_ptr(row) {}

int& Matrix::Row::operator[](size_t index)
{
	if (index >= columns)
		throw std::out_of_range("Wrong index");

	return row_ptr[index];
}

const int& Matrix::Row::operator[](size_t index) const
{
	if (index >= columns)
		throw std::out_of_range("Wrong index");

	return row_ptr[index];
}

Matrix::Matrix(size_t rows_count, size_t columns_count)
		:matrix(new int[rows * columns]()),
		 rows(rows_count),
		 columns(columns_count) {}

Matrix::Matrix(const Matrix &m)
		:matrix(new int[rows * columns]), rows(m.rows), columns(m.columns)
{
	std::copy(m.matrix, m.matrix + rows * columns, matrix);
}

size_t Matrix::getRows() const
{
	return rows;
}

size_t Matrix::getColumns() const
{
	return columns;
}

bool Matrix::operator==(const Matrix& other) const
{
	if (this == &other)
		return true;

	if (rows != other.getRows() || columns != other.getColumns())
		return false;

	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < columns; ++j)
			if (matrix[i * columns + j] != other[i][j])
			return false;

	return true;
}

bool Matrix::operator!=(const Matrix& other) const
{
	return !(*this == other);
}

Matrix& Matrix::operator*=(const int& a)
{
	for (size_t i = 0; i < rows * columns; ++i)
		matrix[i] *= a;

	return *this;
}

Matrix::Row Matrix::operator[](size_t index)
{
	if (index >= rows)
		throw std::out_of_range("Wrong index");

	return Row(matrix + index * columns, columns);
}

const Matrix::Row Matrix::operator[](size_t index) const
{
	if (index >= rows)
		throw std::out_of_range("Wrong index");

	return Row(matrix + index * columns, columns);
}
