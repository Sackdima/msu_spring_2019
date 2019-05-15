all: sort data print

bench: sort
	time ./sort numbers.dat sorted_numbers.dat

sort: big_file_sort.cpp
	g++ big_file_sort.cpp -o sort -O2

data: gen_data.cpp
	g++ gen_data.cpp -o gen -O2

print: print_file.cpp
	g++ print_file.cpp -o print -O2

clear:
	rm -Rf *.cache