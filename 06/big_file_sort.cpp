#include <fstream>
#include <iostream>
#include <cstdio>

#include <string>
#include <cstring>
#include <queue>
#include <algorithm>

#include <thread>
#include <mutex>


const int NUM_THREADS = 2;
//Count of numbers, allowed to read at once
const int CHUNK_SIZE = 10000;

std::mutex m;
size_t InProcess = 0;
//Allow threads to end
bool EndWork = false;

//Queue contains filenames for files to be merged
std::queue<char*> filenames;

//Merge of 2 files to given output file
void merge(std::ifstream& in1, std::ifstream& in2, std::ofstream& out)
{
	uint64_t a, b;

	in1.read(reinterpret_cast<char*> (&a), sizeof(uint64_t));
	in2.read(reinterpret_cast<char*> (&b), sizeof(uint64_t));
	int n = in1.gcount();
	int m = in2.gcount();

	while (n && m)
	{
		if (a > b)
		{
			out.write(reinterpret_cast<char*> (&b), sizeof(uint64_t));
			in2.read(reinterpret_cast<char*> (&b), sizeof(uint64_t));
			m = in2.gcount();
		}
		else
		{
			out.write(reinterpret_cast<char*> (&a), sizeof(uint64_t));
			in1.read(reinterpret_cast<char*> (&a), sizeof(uint64_t));
			n = in1.gcount();
		}
	}
	while (n)
	{
		out.write(reinterpret_cast<char*> (&a), sizeof(uint64_t));
		in1.read(reinterpret_cast<char*> (&a), sizeof(uint64_t));
		n = in1.gcount();
	}
	while (m)
	{
		out.write(reinterpret_cast<char*> (&b), sizeof(uint64_t));
		in2.read(reinterpret_cast<char*> (&b), sizeof(uint64_t));
		m = in2.gcount();
	}
}

void merge_thread()
{
	std::ifstream in1;
	std::ifstream in2;
	char* filename;
	while (!EndWork)
	{
		m.lock();
		if (filenames.size() > 1)
		{
			InProcess++;
			filename = filenames.front();
			in1.open(filename, std::ios::binary);
			if (!in1.is_open())
			{
				std::cerr << "Can't open " << filename << '\n';
			}
			filenames.pop();
			delete[] filename;
			filename = filenames.front();
			in2.open(filename, std::ios::binary);
			if (!in2.is_open())
			{
				std::cerr << "Can't open " << filename << '\n';
			}
			filenames.pop();
			delete[] filename;
		}
		m.unlock();

		if (in1.is_open() && in2.is_open())
		{
			std::string tmp_filename(std::tmpnam(nullptr));
			tmp_filename += ".big_file_sort_cache";
			filename = new char[tmp_filename.size() + 1];
			std::strcpy(filename, tmp_filename.c_str());

			std::ofstream out(filename, std::ios::binary);
			merge(in1, in2, out);
			out.close();
			in1.close();
			in2.close();

			m.lock();
			filenames.emplace(filename);
			InProcess--;
			m.unlock();
		}
	}

}

int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Wrong number of arguments.\n";
		std::cerr << "Usage:\n";
		std::cerr << "		sort in_file_name out_file_name\n";
		return 1;
	}

	std::ifstream in;
	std::ofstream out;
	in.open(argv[1], std::ios::binary);
	out.open(argv[2], std::ios::binary);
	if (!in.is_open())
	{
		std::cerr << "Can't open " << argv[1] << '\n';
		return 1;
	}
	if (!out.is_open())
	{
		std::cerr << "Can't open " << argv[2] << '\n';
		return 1;
	}

	// Create merge threads
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		std::thread t(merge_thread);
		t.detach();
	}

	//Read and sort original file in chunks
	//n - amount of bytes extracted from file
	int n = 8;
	uint64_t chunk[CHUNK_SIZE];
	char * filename;

	while (n)
	{
		in.read(reinterpret_cast<char*>(chunk), sizeof(uint64_t) * CHUNK_SIZE);
		n = in.gcount();
		if (n % 8 != 0)
		{
			std::cout << "Corrupted file" << std::endl;
			return 1;
		}
		if (n)
		{
			std::sort(&chunk[0], &chunk[n / sizeof(uint64_t)]);
			std::string tmp_filename(std::tmpnam(nullptr));
			tmp_filename += ".big_file_sort_cache";
			filename = new char[tmp_filename.size() + 1];
			std::strcpy(filename, tmp_filename.c_str());

//			std::cout << filename << '\n';
			std::ofstream tmp(filename, std::ios::binary);
			if (!tmp.is_open())
			{
				std::cerr << "Can't open " << filename << '\n';
				return 1;
			}
			tmp.write(reinterpret_cast<char*>(chunk), n);
			tmp.close();
			filenames.emplace(filename);

		}
	}
	in.close();

	while (true)
	{
		std::lock_guard<std::mutex> lock(m);
		if (!(InProcess || filenames.size() > 1))
			break;
	}
	EndWork = true;

	filename = filenames.front();
	in.open(filename, std::ios::binary);
	filenames.pop();
	if (!in.is_open())
	{
		std::cerr << "Can't open " << filename << '\n';
		return 1;
	}
	delete[] filename;

	n = 8;
	while (n)
	{
		in.read(reinterpret_cast<char*>(chunk), sizeof(uint64_t) * CHUNK_SIZE);
		n = in.gcount();
		if (n)
		{
			out.write(reinterpret_cast<char*>(chunk), n);
		}
	}
	in.close();
	out.close();

	return 0;
}