#include "Utils.h"

void printPrimesList(const bool* list, unsigned long size) {
	ofstream out;
	out.open("primes-list.txt");

	for (unsigned long i = 0; i < size; i++){
		if(i % 16 == 0)
			out << endl;
		if (list[i])
			out << i + 2 << " ";

	}

	out.clear();
}

bool* newList(unsigned long n) {
	bool* list = (bool*) malloc(n * sizeof(bool));

	fill_n(list, n, true);

	return list;
}
