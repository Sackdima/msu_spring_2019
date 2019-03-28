#ifndef _MATRIX_
#define _MATRIX_

class Matrix {
private:
	size_t rows;
	size_t columns;
	int* matrix;
public:
	Matrix (int rows_count = 1, int cols_count = 1) {
		if (rows_count <= 0 || cols_count <= 0) {
			throw std::out_of_range("");
		}
		rows = rows_count;
		columns = cols_count;
		matrix = (int*) calloc(rows * columns, sizeof(int));
	}
	~Matrix () { delete[] matrix; }

	class Vector {
	private:
		int* row;
		size_t length;
	public:
		Vector(size_t row_num, size_t rows_count) {
			length = rows_count;
			row = *(matrix + row_num);
		}
		int& operator[](int x) {
			if (x > 0 && x < length)
				throw std::out_of_range("");

			return *(row + x);
		}
	};
	Vector& operator[](int ind) {
		if (ind < 0 || ind >= rows)
			throw std::out_of_range("");
		return Vector(ind, rows, matrix);
	}
	bool operator==(const Matrix& other) const {
		if (this == &other)
            return true;

        for (size_t i = 0; i < rows; ++i)
			for (size_t j = 0; j < columns; ++j)
            	if (*(*(matrix + i) + j) != other[i][j])
                	return false;

        return true;
	}
	bool operator!=(const Matrix& other) const {
		return !(*this == other);
	}
	Matrix& operator*=(int a) {
		for (size_t i = 0; i < rows; ++i) {
			for (size_t j = 0; j < columns; j++) {
				*(matrix + i * columns + j) *= a;
			}
		}
		return this;
	}
	size_t getRows() const {
		return rows;
	}
	size_t getColumns() const {
		return rows;
	}
};

#endif
