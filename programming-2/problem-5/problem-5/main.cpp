#include <iostream>
#include <fstream>

#include "crawler.h"
using namespace std;


int main()
{
	ifstream input("input.txt");
	ofstream output("output.txt");

	start_indexing(input, output);

	input.close();
	output.close();

	return EXIT_SUCCESS;
}
