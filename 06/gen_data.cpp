#include <fstream>
#include <iostream>
#include <random>


template <uint64_t MaxNum>
uint64_t getNumber()
{
	static std::random_device rd;
	static std::uniform_int_distribution<uint64_t> dist(0, MaxNum);
	return dist(rd);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Wrong number of arguments!\n";
		return 0;
	}

	std::ofstream out(argv[1], std::ios::binary);
	if (!out)
	{
		std::cerr << "Can't open " << argv[1] << '\n';
		return 1;
	}

	const size_t Count = 100000;
	const uint64_t MaxNumber= 100000;

	for (size_t i = 0; i < Count; ++i)
	{
		uint64_t num = getNumber<MaxNumber>();
		out.write(reinterpret_cast<char *> (&num), sizeof(uint64_t));
	}
	out.close();

	return 0;
}