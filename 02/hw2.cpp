#include <iostream>


char *get_next(char *str) {
	while (*str == ' ')
		str++;

	return str;
}

void print_error() {
	std::cout<< "error"<<'\n';
}

int main(int argc, char *argv[]) {
	if (argc != 2)
		print_error();
	else {

	}

	return 0;
}