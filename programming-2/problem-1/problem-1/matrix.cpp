#include <cstdio>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
using namespace std;


class Matrix {
	int** content;
	size_t dimension;

	int** allocate_memory_for_matrix(size_t dim)
	{
		int** matrix = new int* [dim];

		for (size_t i = 0; i < dim; i++)
		{
			matrix[i] = new int[dim];
		}

		return matrix;
	}

	void copy_matrix(int** dest, int** source, size_t dim)
	{
		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				dest[i][j] = source[i][j];
			}
		}
	}

	void delete_matrix(int** source, size_t dim)
	{
		for (size_t i = 0; i < dim; i++)
		{
			delete[] source[i];
		}
		delete[] source;
	}

public:
	Matrix() : dimension(0), content(nullptr) {}

	Matrix(const size_t dim, int* diag_elements) : dimension(dim), content(allocate_memory_for_matrix(dim))
	{
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
	Matrix(const size_t dim) : dimension(dim), content(allocate_memory_for_matrix(dim))
	{
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

	Matrix(const Matrix& that) : dimension(that.dimension), content(allocate_memory_for_matrix(that.dimension))
	{
		copy_matrix(this->content, that.content, this->dimension);
	}

	Matrix(const size_t dim, ifstream& in) : dimension(dim), content(allocate_memory_for_matrix(dim))
	{
		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				in >> this->content[i][j];
			}
		}
	}

	// matrix dimension getter
	size_t dim() const { return this->dimension; }

	Matrix& operator=(const Matrix& that)
	{
		if (*this != that)
		{
			delete_matrix(this->content, this->dimension);

			this->dimension = that.dimension;
			this->content = allocate_memory_for_matrix(this->dimension);

			copy_matrix(this->content, that.content, this->dimension);
		}

		return *this;
	}

	// matrix product
	Matrix operator*(const Matrix& that) const
	{
		if (that.dimension != this->dimension)
		{
			throw "Can't multiple matrixes with different dimensions.";
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

	Matrix operator*(const int constant) const
	{
		Matrix result(*this);

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				result.content[i][j] *= constant;
			}
		}

		return result;
	}

	//matrix sum
	Matrix operator+(const Matrix& that) const
	{
		if (that.dimension != this->dimension)
		{
			throw "Can't add matrixes with different dimensions.";
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

	Matrix operator-()
	{
		return (*this) * (-1);
	}

	void operator+=(const Matrix& that)
	{
		*this = *this + that;
	}

	void operator-=(const Matrix& that)
	{
		*this = *this - that;
	}

	void operator*=(const Matrix& that)
	{
		*this = *this * that;
	}

	void operator*=(const int constant)
	{
		*this = (*this) * constant;
	}

	//matrix equality
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

	// transponse matrix
	Matrix operator~() const
	{
		Matrix result(*this);

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = i + 1; j < this->dimension; j++)
			{
				if (i != j)
				{
					swap(result.content[i][j], result.content[j][i]);
				}
			}
		}

		return result;
	}

	Matrix operator()(size_t row, size_t column) const
	{
		if (row > this->dimension || column > this->dimension)
		{
			throw "Can't create minor: column or row number greater than dimension.";
		}

		row--;
		column--;

		Matrix result(this->dimension - 1);

		for (size_t i = 0; i < this->dimension - 1; i++)
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

	int*& operator[](const size_t index) {
		if (index > this->dimension) {
			throw "Row index greater than dimension.";
		}

		return this->content[index - 1];
	}

	//void operator()(const size_t index)
	//{
	//	reference_wrapper<int> a;
	//}

	void print() const
	{
		cout << endl;

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				cout << this->content[i][j] << " ";
			}
			cout << endl;
		}
	}

	void print(string filename) const
	{
		ofstream out;
		out.open(filename);

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				out << this->content[i][j] << " ";
			}
			out << endl;
		}

		out.close();
	}

	~Matrix()
	{
		delete_matrix(this->content, this->dimension);
	}

};


Matrix operator*(int constant, const Matrix& A)
{
	return A * constant;
}


int main()
{
	string input_filename = "input.txt";
	ifstream input(input_filename);
	size_t dimension;
	int k;

	input >> dimension;
	input >> k;

	Matrix A(dimension, input), B(dimension, input), C(dimension, input), D(dimension, input), K(dimension);
	input.close();

	K *= k;

	((A + B * (~C) + K) * (~D)).print("output.txt");
	A.print();
	B.print();

	A += B;
	A.print();
	A[2][2] += 5;
	A(2)[2] -= 5;
	cout << A[1][1];
	A.print();

	return EXIT_SUCCESS;
}
