#include <fstream>
#include <cstdio>
#include <iostream>
using namespace std;

#include "matrix.h"

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

	A.print();
	A[2][1] -= 10;
	A.print();
	A(1)[2] -= 5;
	A.print();

	((A + B * (~C) + K) * (~D)).print("output.txt");

	return EXIT_SUCCESS;
}