#include <iostream>


enum Operations {
	PLUS = 1,
	MINUS,
	MUL,
	DIV,
	DIGIT
};

// bool check_and_convert(char* src, char* dst) {
//
//
// 	return res;
// }

void get_num(char* &str, int* num) {
	int res = 0;


	while (isdigit(*str) || *str == ' ') {
		if (*str != ' ') {
			res *= 10;
			res += *str - '0';
		}
		str++;
	}
	*num = res;
}

char* get_next(char* str) {
	while (*str == ' ')
		str++;

	return str;
}

void print_error() {
	std::cout<< "error"<<'\n';
}

int main(int argc, char* argv[]) {
	if (argc != 2)
		print_error();
	else {
		char* str;
		int num;

		str = argv[1];
		get_num(str, &num);
		std::cout << num << '\n';
		std::cout << str << '\n';
	}

	return 0;
}
