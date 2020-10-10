#include <fstream>
#include <cstdio>
#include <iostream>
using namespace std;

#include "matrix.h"

int main()
{
	ifstream input("input.txt");
	size_t dimension;
	int k;

	input >> dimension;
	input >> k;

	Matrix A(dimension), B(dimension), C(dimension), D(dimension), K(dimension);
	A >> input;
	B >> input;
	C >> input;
	D >> input;
	input.close();

	K *= k;

	ofstream output("output.txt");
	(A + B * (~C) + K) * (~D) << output;
	output.close();

	return EXIT_SUCCESS;
}