#include <fstream>
#include <iostream>
#include <cstdio>

#include <string>
#include <queue>
#include <algorithm>

#include <thread>
#include <mutex>


const int CHUNK_SIZE = 10000;
std::mutex m;
size_t InProcess = 0;
bool FileEnded = false;
bool EndWork = false;
bool EndedMerge = false;

void merge(std::ifstream& in1, std::ifstream& in2, std::ofstream& out)
{

}

//void merge_thread()
//{
//	while (!EndWork)
//	{
//		if(std::try_lock(m))
//		{
//			if (filenames.size() > 1)
//			{
//				std::ifstream in1(filenames.front(), std::ios::binary);
//				filenames.pop();
//				std::ifstream in2(filenames.front(), std::ios::binary);
//				filenames.pop();
//				std::string filename(std::tmpnam(nullptr));
//				filename += ".cache";
//				std::ofstream out(filename, std::ios::binary);
//				merge(in1, in2, out);
//				filenames.push(filename.c_str());
//			}
//		}
//	}
//
//}

int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments\n";
		return 0;
	}

	std::ifstream in(argv[1], std::ios::binary);
	std::ofstream out(argv[2], std::ios::binary);
	if (!in)
	{
		std::cerr << "Can't open " << argv[1] << '\n';
		return 1;
	}
	if (!out)
	{
		std::cerr << "Can't open " << argv[2] << '\n';
		return 1;
	}

	int n = 8;
	std::queue<const char*> filenames;
	while (n)
	{
		uint64_t chunk[CHUNK_SIZE];
		in.read(reinterpret_cast<char*>(chunk), sizeof(uint64_t) * CHUNK_SIZE);
		n = in.gcount();
		std::cout<<"-----------" << n << "------------\n";
		if (n % 8 != 0)
		{
			std::cout << "Corrupted file" << std::endl;
			return 1;
		}
		if (n)
		{
			std::sort(&chunk[0], &chunk[n/8]);
			std::string filename(std::tmpnam(nullptr));
			filename += ".cache";
			std::cout << filename << '\n';
			std::ofstream tmp(filename, std::ios::binary);
			if (!tmp)
			{
				std::cerr << "Can't open " << filename << '\n';
				return 1;
			}
			out.write(reinterpret_cast<char*>(chunk), n);
			out.close();
			filenames.push(filename.c_str());
			EndedMerge = true;
		}
		else
		{
			FileEnded = true;
		}
	}

	while (!EndedMerge)
		;
	if(filenames.empty())
	{
		std::cerr << "empty queue" << '\n';
		return 1;
	}
	const char* filename = filenames.front();
	filenames.pop();
	std::ifstream result(filename, std::ios::binary);
	n = 8;
	while (n)
	{
		uint64_t chunk[CHUNK_SIZE];
		result.read(reinterpret_cast<char*>(chunk), sizeof(uint64_t) * CHUNK_SIZE);
		n = in.gcount();
		std::cout<<"-----------" << n << "------------\n";
		if (n % 8 != 0)
		{
			std::cout << "Corrupted file" << std::endl;
			break;
		}
		if (n)
		{
			out.write(reinterpret_cast<char*>(chunk), n);
		}
	}
	in.close();
	result.close();
	out.close();

	return 0;
}