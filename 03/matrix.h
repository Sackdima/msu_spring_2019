#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <cstdlib>

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
	~Matrix() { delete[] matrix; }

	size_t getRows() const;
	size_t getColumns() const;

	bool operator==(const Matrix& other) const;
	bool operator!=(const Matrix& other) const;
	Matrix operator*=(const int& a);

	Row operator[](int index);
	Row operator[](int index) const;
};

#endif
