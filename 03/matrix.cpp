#include "matrix.h"

int& Matrix::Row::operator[](int index)
{
	if (index < 0 || index >= columns)
		throw std::out_of_range("Wrong index");

	return row_ptr[index];
}

Matrix::Matrix(size_t rows_count, size_t columns_count)
{
	if (rows_count < 0 || columns_count < 0)
		throw std::out_of_range("Can't create matrix with size 0");

	rows = rows_count;
	columns = columns_count;
	matrix = new int[rows * columns]();
}

Matrix::Matrix(const Matrix &m)
		:matrix(new int[rows * columns]), rows(m.rows), columns(m.columns)
{
	// rows = m.rows;
	// columns = m.columns;
	// matrix = (int *) calloc(rows * columns, sizeof(int));

	std::copy(m.matrix, m.matrix + rows * columns, matrix);
}

size_t Matrix::getRows() const {
	return rows;
}

size_t Matrix::getColumns() const {
	return columns;
}

bool Matrix::operator==(const Matrix& other) const {
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

bool Matrix::operator!=(const Matrix& other) const {
	return !(*this == other);
}

Matrix Matrix::operator*=(const int& a) {
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < columns; ++j)
			*(matrix + i * columns + j) *= a;
	Matrix tmp = *this;

	return tmp;
}

Matrix::Row Matrix::operator[](int index) {
	if (index < 0 || index >= rows)
		throw std::out_of_range("Wrong index");

	return Row(matrix + index * columns, columns);
}

Matrix::Row Matrix::operator[](int index) const {
	if (index < 0 || index >= rows)
		throw std::out_of_range("Wrong index");

	return Row(matrix + index * columns, columns);
}
