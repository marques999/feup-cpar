#include "common.h"

void sieveSequential(uint64_t power, ofstream& out) {
	int rank, size;
	double openMPITime = 0;
	bool* list;
	uint64_t startBlockValue, counter = 0, primes = 0, n = pow(2, power);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	uint64_t blockSize = BLOCK_SIZE(rank, n - 1, size);
	uint64_t bottom = BLOCK_LOW(rank, n - 1, size) + 2;
	uint64_t upper = BLOCK_HIGH(rank, n - 1, size) + 2;

	list = newList(blockSize);

	MPI_Barrier (MPI_COMM_WORLD);

	if(rank == 0) {
		out << size << ";" << n << ";" << power << ";";
		openMPITime = -MPI_Wtime();
	}

	uint64_t p = 2;
	while(p * p <= n) {

		if ( (p * p) < bottom) {
			if(bottom % p == 0){
				startBlockValue = bottom;
			} else {
				startBlockValue = bottom + (p - (bottom % p));
			}
		} else {
			startBlockValue = p * p;
		}
		for (uint64_t i = startBlockValue; i <= upper; i += p)
			list[i - bottom] = false;

		if (rank == 0) {
			for(uint64_t i = p + 1; i < upper; i++) {
				if (list[i - bottom]) {
					p = i;
					break;
				}
			}
		}

		MPI_Bcast(&p, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	}

	if(rank == 0) {
		openMPITime += MPI_Wtime();
		out << openMPITime << ";";
	}

	for (uint64_t i = 0; i < blockSize; i++)
		if (list[i])
			counter++;

	if (size > 1)
		MPI_Reduce(&counter, &primes, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	else
		primes = counter;

	if(rank == 0)
		out << primes << endl;

	free(list);
}

void sieveParallel(unsigned long power, int threads, ofstream& out) {
	int rank, size;
	double openMPITime = 0;
	bool* list;
	uint64_t startBlockValue, counter = 0, primes = 0, n = pow(2, power);

	omp_set_num_threads(threads);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	uint64_t blockSize = BLOCK_SIZE(rank, n - 1, size);
	uint64_t bottom = BLOCK_LOW(rank, n - 1, size) + 2;
	uint64_t upper = BLOCK_HIGH(rank, n - 1, size) + 2;

	list = newList(blockSize);

	MPI_Barrier (MPI_COMM_WORLD);

	if (rank == 0) {
		out << size << ";" << n << ";" << power << ";" << threads << ";";
		openMPITime = -MPI_Wtime();
	}

	uint64_t p = 2;
	while( (p * p) <= n) {
		if ( (p * p) < bottom) {
			if(bottom % p == 0){
				startBlockValue = bottom;
			} else {
				startBlockValue = bottom + (p - (bottom % p));
			}
		} else {
			startBlockValue = p * p;
		}

#pragma omp parallel for num_threads(threads)
		for (uint64_t i = startBlockValue; i <= upper; i += p)
			list[i - bottom] = false;

		if (rank == 0) {
			for(uint64_t i = p + 1; i < upper; i++) {
				if (list[i - bottom]) {
					p = i;
					break;
				}
			}
		}

		MPI_Bcast(&p, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	}

	if (rank == 0) {
		openMPITime += MPI_Wtime();
		out << openMPITime << ";";
	}

	for (uint64_t i = 0; i < blockSize; i++)
		if (list[i])
			counter++;

	if (size > 1)
		MPI_Reduce(&counter, &primes, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	else
		primes = counter;

	if(rank == 0)
		out << primes << endl;

	free(list);
}

int main(int argc, char** argv) {

	ofstream out;

	MPI_Init(&argc, &argv);

	uint64_t n = (uint64_t) atol(argv[1]);

	if(argv[2]){
	  out.open ("mpiparallel.csv", ios::app);
		int threads = (int) atoi(argv[2]);
		sieveParallel(n, threads, out);
	}
	else{
		out.open ("mpisequential.csv", ios::app);
		sieveSequential(n, out);
	}

	MPI_Finalize();

	out.close();

	return 0;
}
