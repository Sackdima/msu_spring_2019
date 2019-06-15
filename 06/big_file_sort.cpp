#include <fstream>
#include <iostream>
#include <cstdio>
#include <unistd.h>

#include <string>
#include <cstring>
#include <queue>
#include <algorithm>

#include <thread>
#include <mutex>


//Count of threads ( can only be 2)
const int NUM_THREADS = 2;
//Count of numbers, allowed to read at once
const int MAX_CHUNK_SIZE = 1000;
const int CHUNK_SIZE = MAX_CHUNK_SIZE / NUM_THREADS / 2; // so sum threads memory won't be more then MAX_CHUNK_SIZE

std::mutex m;
size_t InProcess = 0;
bool SplitEnded = false;

//Merge of 2 files to given output file
void merge(int src1, int src2, int dst)
{
	uint64_t a, b;
	int n, k, w; // w -amount of bytes written, debug


	n = read(src1, &a, sizeof(uint64_t));
	k = read(src2, &b, sizeof(uint64_t));

	while (n && k)
	{
		if (a > b)
		{
			w = write(dst, &b, sizeof(uint64_t));
			k = read(src2, &b, sizeof(uint64_t));
		}
		else
		{
			w = write(dst, &a, sizeof(uint64_t));
			n = read(src1, &a, sizeof(uint64_t));
		}
	}
	while (n)
	{
		w = write(dst, &a, sizeof(uint64_t));
		n = read(src1, &a, sizeof(uint64_t));
	}
	while (k)
	{
		w = write(dst, &b, sizeof(uint64_t));
		k = read(src2, &b, sizeof(uint64_t));
	}
}

void sort_thread(std::queue<int>& fds)
{
	while (!SplitEnded || fds.size() > 1 || InProcess > 0)
	{
		m.lock();
		if (fds.size() > 1)
		{
			InProcess++;
			int fd1, fd2;
			fd1 = fds.front();
			fds.pop();
			fd2 = fds.front();
			fds.pop();
			m.unlock();

			char filename[] = "/tmp/big_file_sort.XXXXXX";
			int fd = mkstemp(filename);
			if (fd == -1)
				std::cerr << "err: " << filename << '\n'; // not thread-safe, but equals to wrong sort, so anyway wrong result
			unlink(filename);
			merge(fd1, fd2, fd);
			lseek(fd, 0, SEEK_SET);
			close(fd1);
			close(fd2);
			m.lock();
			InProcess--;
			fds.push(fd);
			m.unlock();
		}
		else
		{
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
		std::cerr << "\tsort in_file_name out_file_name\n";
		return 1;
	}

	std::fstream in(argv[1], std::ios::binary | std::fstream::in | std::fstream::out);
	if (!in.is_open())
	{
		std::cerr << "Can't open " << argv[1] << '\n';
		return 1;
	}

	//container for file descriptors
	std::queue<int> fds_to_merge;

	std::thread threads[NUM_THREADS];
	for (auto& i : threads)
	{
		i = std::thread(sort_thread, std::ref(fds_to_merge));
	}

	//n - amount of bytes extracted from file
	int n = 8;
	uint64_t chunk[CHUNK_SIZE];

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
			char filename[] = "/tmp/big_file_sort.XXXXXX";
			int fd = mkstemp(filename);
			if (fd == -1)
				std::cerr << "Can't open " << filename << '\n';
			unlink(filename);
			write(fd, &chunk[0], n);
			lseek(fd, 0, SEEK_SET);
			m.lock();
			fds_to_merge.push(fd);
			m.unlock();
		}
	}
	SplitEnded = true;

	for (auto& i : threads)
	{
		i.join();
	}


	int fd = fds_to_merge.front();
	fds_to_merge.pop();

	if (std::strcmp(argv[1], argv[2]))
	{
		std::ofstream out(argv[2], std::ios::binary | std::ios::trunc);
		int n = read(fd, &chunk[0], CHUNK_SIZE);

		while (n)
		{
			out.write(reinterpret_cast<char*>(chunk), n);
			n = read(fd, &chunk[0], CHUNK_SIZE);
		}
		out.close();
	}
	else
	{
		in.close();
		in.open(argv[2], std::ios::binary | std::ios::trunc | std::ios::out);

		int n = read(fd, &chunk[0], CHUNK_SIZE);

		while (n)
		{
			in.write(reinterpret_cast<char *>(chunk), n);
			n = read(fd, &chunk[0], CHUNK_SIZE);
		}
			in.close();
	}
	close(fd);

	return 0;
}