#include <iostream>
#include "numbers.dat"


bool check_args(int argc, char const *argv[])
{
	for (int i = 1; i < argc; i += 2)
	{
		int first_el = std::atoi(argv[i]);
		int last_el = std::atoi(argv[i + 1]);

		if (first_el <= 0  || last_el <= 0)
			return false;

		return true;
	}
}

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

int count_prime(const int * Data, size_t l, size_t r)
{
	size_t i = l;
	int count = 0;

	while (i <= r)
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
		if (check_args(argc, argv))
		{
			for (int i = 1; i < argc; i += 2)
			{
				int first_el = std::atoi(argv[i]);
				int last_el = std::atoi(argv[i + 1]);
				size_t start = 0;
				size_t end = Size - 1;

				while (start < end)
				{
					if (Data[start] < first_el)
						++start;
					if (Data[end] > last_el)
						--end;
					if (Data[start] >= first_el && Data[end] <= last_el)
						break;
				}
				if (Data[start] != first_el || Data[end] != last_el)
					std::cout << 0 << '\n';
				else
					std::cout << count_prime(Data, start, end) << '\n';
	    	}
		}
		else
		{
			code = -1;
		}
	}

    return code;
}
