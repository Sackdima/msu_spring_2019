#pragma once
#include <fstream>
#include <iostream>


const size_t CHUNK_SIZE = 100;
void print_file(std::ifstream &in)
{
	size_t n = 8;
	while (n)
	{
		uint64_t chunk[CHUNK_SIZE];
		in.read(reinterpret_cast<char*>(chunk), sizeof(uint64_t) * CHUNK_SIZE);
		n = in.gcount();
//		std::cout<<"-----------" << n << "------------\n";
		if (n % 8 != 0)
		{
			std::cout << "Corrupted file!" << std::endl;
			break;
		}

		if (n)
		{
			for (size_t i = 0; i < n/8; ++i)
				std::cout << chunk[i] << '\n';
		}
	}
}
