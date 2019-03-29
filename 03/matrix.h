#ifndef _MATRIX_
#define _MATRIX_

#include <cstdlib>
#include <stdexcept>

class Matrix {
private:
	size_t rows;
	size_t columns;
	int *matrix;

public:
	class Row {
	private:
		size_t columns;
		int *row_ptr;
	public:
		Row(int *row, size_t cols)
				: columns(cols), row_ptr(row) {}

		int &operator[](int index);
	};

	Matrix(size_t rows_count = 1, size_t columns_count = 1);
	Matrix(const Matrix &m);
	~Matrix() { free(matrix); }

	size_t getRows() const;
	size_t getColumns() const;

	bool operator==(const Matrix& other) const;
	bool operator!=(const Matrix& other) const;
	Matrix operator*=(const int& a);

	Row operator[](int index);
	Row operator[](int index) const;
};

#endif
