all: sort data print

sort: big_file_sort.cpp
	g++ big_file_sort.cpp -o sort -O2 -pthread

data: gen_data.cpp
	g++ gen_data.cpp -o gen -O2

print: print_file.cpp print_file.h
	g++ print_file.cpp -o print -O2
