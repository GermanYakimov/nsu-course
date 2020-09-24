#include <cstdio>
#include <algorithm>
#include <iostream>
#include <fstream>
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

	Matrix(size_t dim, ifstream& in): dimension(dim)
	{
		this->content = allocate_memory_for_matrix(dim);

		for (size_t i = 0; i < this->dimension; i++)
		{
			for (size_t j = 0; j < this->dimension; j++)
			{
				in >> this->content[i][j];
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

	Matrix operator~()
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

	//void fill_from_file(ifstream& in)
	//{
	//	for (size_t i = 0; i < this->dimension; i++)
	//	{
	//		for (size_t j = 0; j < this->dimension; j++)
	//		{
	//			in >> this->content[i][j];
	//		}
	//	}
	//}

	//void fill_from_file(string filename)
	//{
	//	ifstream in(filename);

	//	for (size_t i = 0; i < this->dimension; i++)
	//	{
	//		for (size_t j = 0; j < this->dimension; j++)
	//		{
	//			in >> this->content[i][j];
	//		}
	//	}

	//	in.close();
	//}

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
	string input_filename = "input.txt";
	ifstream in(input_filename);
	size_t dimension;
	int k;
	in >> dimension;
	in >> k;
	Matrix A(dimension, in), B(dimension, in), C(dimension, in), D(dimension, in);
	in.close();

	int* k_diag = new int[dimension];
	for (size_t i = 0; i < dimension; i++)
	{
		k_diag[i] = k;
	}

	Matrix K(dimension, k_diag);
	delete[] k_diag;

	((A + B * (~C) + K) * (~D)).print("output.txt");

	return EXIT_SUCCESS;
}
