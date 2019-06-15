#include <fstream>
#include <iostream>
#include <cstdio>

#include <string>
#include <cstring>
#include <queue>
#include <algorithm>

#include <thread>
#include <mutex>


//Count of threads ( can only be 2)
const int NUM_THREADS = 2;
//Count of numbers, allowed to read at once
const int CHUNK_SIZE = 10000;


//Merge of 2 files to given output file
void merge(std::fstream& in1, std::fstream& in2, std::fstream& out)
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

void sort_thread(std::fstream& in, std::fstream& out)
{
	in.seekg(0);
	in.seekp(0);

	int n = 8;
	uint64_t chunk[CHUNK_SIZE];

	if (in.good())
		std::cout << "1\n";
	while (n)
	{
		in.read(reinterpret_cast<char*>(chunk), sizeof(uint64_t) * CHUNK_SIZE);
		n = in.gcount();
		std::cout << n << '\n';
		if (n)
		{
			std::cout << n << '\n';
			std::sort(&chunk[0], &chunk[n / sizeof(uint64_t)]);
			out.write(reinterpret_cast<char*>(chunk), n);
		}
	}

	out.seekg(0);
	out.seekp(0);

}

int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Wrong number of arguments.\n";
		std::cerr << "Usage:\n";
		std::cerr << "\tsort in_file_name out_file_name\n";
		return 1;
	}

	std::fstream in(argv[1], std::ios::binary | std::fstream::in | std::fstream::out | std::ios::ate);	 // ate - to get size of file
	if (!in.is_open())
	{
		std::cerr << "Can't open " << argv[1] << '\n';
		return 1;
	}

	const char* tmp_file_11 = "big_file_sort.tmp11";
	const char* tmp_file_12 = "big_file_sort.tmp12";
	const char* tmp_file_21 = "big_file_sort.tmp21";
	const char* tmp_file_22 = "big_file_sort.tmp22";
	std::fstream tmp11(tmp_file_11, std::ios::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
	std::fstream tmp12(tmp_file_12, std::ios::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
	std::fstream tmp21(tmp_file_21, std::ios::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
	std::fstream tmp22(tmp_file_22, std::ios::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
//	std::fstream tmp11, tmp12, tmp21, tmp22;
//	tmp11.open(tmp_file_11, std::ios::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
//	tmp12.open(tmp_file_12, std::ios::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
//	tmp21.open(tmp_file_21, std::ios::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
//	tmp22.open(tmp_file_22, std::ios::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);

	if (!tmp11.is_open())
	{
		std::cerr << "Can't create " << tmp_file_11 << '\n';
		return 1;
	}
	if (!tmp12.is_open())
	{
		std::cerr << "Can't create " << tmp_file_12 << '\n';
		return 1;
	}
	if (!tmp21.is_open())
	{
		std::cerr << "Can't create " << tmp_file_21 << '\n';
		return 1;
	}
	if (!tmp22.is_open())
	{
		std::cerr << "Can't create " << tmp_file_22 << '\n';
		return 1;
	}

	//split original file to 2 parts
	//n - amount of bytes extracted from file
	int n = 8;
	uint64_t chunk[CHUNK_SIZE];

	size_t file_size = in.tellg();
	if (file_size % 8 != 0)
	{
		std::cout << "Corrupted file" << std::endl;
		return 1;
	}

	in.seekg(0);
	size_t middle = file_size / 2;
	std::cout << file_size / 8 << ' ' <<  middle / 8 << '\n';
	size_t cum_read = 0;// count of read numbers;

	while (n)
	{
		in.read(reinterpret_cast<char*>(chunk), sizeof(uint64_t) * CHUNK_SIZE);
		n = in.gcount();
		if (n)
		{
			cum_read += n;
			if (cum_read < middle)
			{
				tmp11.write(reinterpret_cast<char*>(chunk), n);
			}
			else
			{
				tmp21.write(reinterpret_cast<char*>(chunk), n);
			}
		}
	}

	std::thread t1(sort_thread, std::ref(tmp11), std::ref(tmp12));
	std::thread t2(sort_thread, std::ref(tmp21), std::ref(tmp22));

	t1.join();
	t2.join();

	if (!std::strcmp(argv[1], argv[2]))
	{
		in.close();
		in.open(argv[1], std::ios::binary | std::fstream::out | std::fstream::trunc);
		merge(tmp12, tmp22, in);
	}
	else
	{
		std::fstream out(argv[2], std::ios::binary | std::fstream::out | std::fstream::trunc);
		if (!out.is_open())
		{
			std::cerr << "Can't open " << argv[2] << '\n';
			return 1;
		}
		merge(tmp12, tmp22, out);
		out.close();
	}

	tmp11.close();
	tmp12.close();
	tmp21.close();
	tmp22.close();
	std::remove(tmp_file_11);
	std::remove(tmp_file_12);
	std::remove(tmp_file_21);
	std::remove(tmp_file_22);
	in.close();

	return 0;
}