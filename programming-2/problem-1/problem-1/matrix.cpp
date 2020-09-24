#include <cstdio>
#include <algorithm>
#include <iostream>
using namespace std;


class Matrix {
	int** content;
	size_t dimension;

	int** allocate_memory_for_matrix(size_t size)
	{
		int** matrix = new int*[size];

		for (size_t i = 0; i < size; i++)
		{
			matrix[i] = new int[size];
		}

		return matrix;
	}

public:
	Matrix() : dimension(0), content(nullptr) {}

	Matrix(size_t dim, int* diag_elements): dimension(dim)
	{
		this->content = allocate_memory_for_matrix(dim);

		for (size_t i = 0; i < dim; i++)
		{
			for (size_t j = 0; j < dim; j++)
			{
				if (i == j)
				{
					this->content[i][j] = diag_elements[i];
				}
				else
				{
					this->content[i][j] = 0;
				}
			}
		}
	}

	// Initializing I here
	Matrix(size_t dim) : dimension(dim)
	{
		this->content = allocate_memory_for_matrix(dim);

		for (size_t i = 0; i < dim; i++)
		{
			for (size_t j = 0; j < dim; j++)
			{
				if (i == j)
				{
					this->content[i][j] = 1;
				}
				else
				{
					this->content[i][j] = 0;
				}
			}
		}
	}

	Matrix(const Matrix& that): dimension(that.dimension)
	{
		this->content = allocate_memory_for_matrix(this->dimension);

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				this->content[i][j] = that.content[i][j];
			}
		}
	}

	Matrix& operator=(const Matrix& that)
	{
		if (*this != that)
		{
			for (size_t i = 0; i < this->dimension; i++)
			{
				delete[] this->content[i];
			}
			delete[] this->content;

			this->dimension = that.dimension;
			this->content = allocate_memory_for_matrix(this->dimension);

			for (size_t i = 0; i < this->dimension; i++)
			{
				for (size_t j = 0; j < this->dimension; j++)
				{
					this->content[i][j] = that.content[i][j];
				}
			}
		}
		return *this;
	}

	Matrix operator*(const Matrix& that) const
	{
		if (that.dimension != this->dimension)
		{
			throw;
		}

		Matrix result(this->dimension);

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				result.content[i][j] = 0;

				for (size_t k = 0; k < this->dimension; k++)
				{
					result.content[i][j] += this->content[i][k] * that.content[k][j];
				}
			}
		}

		return result;
	}

	Matrix operator*(int cofactor) const
	{
		Matrix result(*this);

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				result.content[i][j] *= cofactor;
			}
		}

		return result;
	}

	Matrix operator+(const Matrix& that) const
	{
		if (that.dimension != this->dimension)
		{
			throw;
		}

		Matrix result(this->dimension);

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				result.content[i][j] = this->content[i][j] + that.content[i][j];
			}
		}

		return result;
	}

	Matrix operator-(const Matrix& that) const
	{
		Matrix tmp = that;
		tmp = tmp * (-1);

		return tmp + *this;
	}

	bool operator==(const Matrix& that) const
	{
		if (this->dimension != that.dimension)
		{
			return false;
		}

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				if (this->content[i][j] != that.content[i][j])
				{
					return false;
				}
			}
		}

		return true;
	}

	bool operator!=(const Matrix& that) const
	{
		return !(*this == that);
	}

	void operator~()
	{
		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				if (i != j)
				{
					swap(this->content[i][j], this->content[j][i]);
				}
			}
		}
	}

	Matrix operator()(size_t row, size_t column) const
	{
		if (row > this->dimension || column > this->dimension)
		{
			throw;
		}
		row--;
		column--;

		Matrix result(this->dimension - 1);

		for (size_t i = 0; i < this->dimension - 1 ; i++)
		{
			for (size_t j = 0; j < this->dimension - 1; j++)
			{
				if (i < row && j < column)
				{
					result.content[i][j] = this->content[i][j];
				}
				else if (i < row && j >= column)
				{
					result.content[i][j] = this->content[i][j + 1];
				}
				else if (i >= row && j < column)
				{
					result.content[i][j] = this->content[i + 1][j];
				}
				else
				{
					result.content[i][j] = this->content[i + 1][j + 1];
				}
			}
		}

		return result;
	}

	void print() const
	{
		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				cout << this->content[i][j] << " ";
			}
			cout << endl;
		}
	}

	~Matrix()
	{
		for (size_t i = 0; i < this->dimension; i++)
		{
			delete[] this->content[i];
		}
		delete[] this->content;
	}

};


Matrix operator*(int cofactor, const Matrix& A)
{
	return A * cofactor;
}


int main()
{
	Matrix A(10);
	A.print();
	A = 3 * A * (2 * A);
	cout << endl;
	A.print();

	return EXIT_SUCCESS;
}
