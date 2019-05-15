#include <fstream>
#include <iostream>
#include "print_file.h"


int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Wrong number of arguments!\n";
		std::cerr << "Usage:\n";
		std::cerr << "		print in_file_name\n";
		return 1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cerr << "Can't open " << argv[1] << '\n';
		return 1;
	}
	print_file(in);
	return 0;
}