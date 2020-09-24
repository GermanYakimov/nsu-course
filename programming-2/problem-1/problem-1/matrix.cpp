
class Matrix {
	int** content;

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
	const size_t dimension;

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

	~Matrix()
	{
		for (size_t i = 0; i < this->dimension; i++)
		{
			delete[] this->content[i];
		}
		delete[] this->content;
	}

};
