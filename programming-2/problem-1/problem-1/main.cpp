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
	input >> A >> B >> C >> D;
	input.close();

	K *= k;

	ofstream output("output.txt");
	output << (A + B * (~C) + K) * (~D);
	output.close();

	return EXIT_SUCCESS;
}