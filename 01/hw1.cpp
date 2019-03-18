#include <iostream>
#include "numbers.dat"


bool is_prime(size_t n)
{
	bool res = true;
	size_t i = 5;

	if (n == 2 || n == 3)
		return true;
	if (!(n % 2 && n % 3) || n == 1)
		return false;

	while (i * i <= n && res)
	{
		if ( !(n % i))
			res = false;
		i += 2;
	}

	return res;
}

int count_prime(const int * Data, size_t start, size_t end)
{
	size_t i = start;
	int count = 0;

	while (i <= end)
	{
		if (is_prime(Data[i]))
			++count;
		++i;
	}

	return count;
}

int main(int argc, char const *argv[])
{
	int code = 0; // return code

	if (!(argc  & 1) || argc == 1)
		code = -1;
	else
	{
		for (int i = 1; i < argc; i += 2)
		{
			int first_num = std::atoi(argv[i]);
			int last_num = std::atoi(argv[i + 1]);
			size_t start = 0;
			size_t end = Size - 1;

			if (first_num <= 0 || last_num <= 0)
			{
				code = -1;
				break;
			}
			while (start < end)
			{
				if (Data[start] < first_num)
					++start;
				if (Data[end] > last_num)
					--end;
				if (Data[start] >= first_num && Data[end] <= last_num)
					break;
			}
			if (Data[start] != first_num || Data[end] != last_num)
				std::cout << 0 << '\n';
			else
				std::cout << count_prime(Data, start, end) << '\n';
		}
	}

    return code;
}
