#include "papi.h"
#include <openmpi/mpi.h>

#define BLOCK_LOW(i,n,p) ((i)*(n)/(p))
#define BLOCK_HIGH(i,n,p) (BLOCK_LOW((i)+1,n,p)-1)
#define BLOCK_SIZE(i,n,p) (BLOCK_LOW((i)+1,n,p)-BLOCK_LOW(i,n,p))
#define BLOCK_OWNER(index,n,p) ((((p)*(index)+1)-1)/(n))

typedef struct timespec nanoTime;

static double getElapsed(nanoTime start, nanoTime finish)
{
	return (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(int argc, char* argv[])
{
	int rank;
	int size;
	int root = 0;

	if (argc != 2)
	{
		std::cerr << "[ error ] Missing argument N (exponent)...\n";
		exit(1);
	}

	long long performanceValues[3];
	long long maximumExponent = atol(argv[1]);

	if (maximumExponent < 1)
	{
		std::cerr << "[ error ] 2^N must be equal or greater than 1!\n";
		exit(1);
	}

	if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
	{
		return 1;
	}

	if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
	{
		return MPI_Finalize();
	}

	if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
	{
		return MPI_Finalize();
	}

	if (PAPI_Initialize() != PAPI_OK)
	{
		return MPI_Finalize();
	}

	if (PAPI_Begin() != PAPI_OK)
	{
		return PAPI_Destroy() && MPI_Finalize();
	}

	nanoTime clockStart;
	nanoTime clockFinish;

	if (rank == root)
	{
		clock_gettime(CLOCK_MONOTONIC, &clockStart);
	}

	uint64_t j;
	uint64_t k = 2;
	uint64_t largestPrime = pow(2, maximumExponent);
	uint64_t blockLength = BLOCK_SIZE(rank, largestPrime - 1, size);
	uint64_t blockLow = 2 + BLOCK_LOW(rank, largestPrime - 1, size);
	uint64_t blockHigh = 2 + BLOCK_HIGH(rank, largestPrime - 1, size);

	bool* v = new bool[blockLength];

	for (uint64_t i = 0; i < blockLength; i++)
	{
		v[i] = true;
	}

	std::cout << "\nRank: " << rank
		      << "\nSize: " << BLOCK_SIZE(rank, largestPrime, size)
			  << "\nLow: " << BLOCK_LOW(rank, largestPrime, size)
			  << "\nHigh: " << BLOCK_HIGH(rank, largestPrime, size)
			  << std::endl;

	while (k * k <= largestPrime)
	{
		if (k * k < blockLow)
		{
			if (blockLow % k == 0)
			{
				j = blockLow;
			}
			else
			{
				j = blockLow + (k - (blockLow % k));
			}

		}
		else
		{
			j = k * k;
		}

		for (uint64_t i = j; i <= blockHigh; i += k)
		{
			v[i - blockLow] = false;
		}

		if (rank == root)
		{
			for (uint64_t i = k + 1; i < blockHigh; i++)
			{
				if (v[i - blockLow])
				{
					k = i;
					break;
				}
			}
		}

		MPI_Bcast(&k, 1, MPI_INT, root, MPI_COMM_WORLD);
	}

	uint64_t totalPrimes = 0;
	uint64_t blockPrimes = 0;

	for (uint64_t i = 0; i < blockLength; i++)
	{
		if (v[i])
		{
			blockPrimes++;
		}
	}

	MPI_Reduce(&blockPrimes, &totalPrimes, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

	if (rank == root)
	{
		clock_gettime(CLOCK_MONOTONIC, &clockFinish);
		std::cout << "2,"
				  << maximumExponent << ","
				  << size << ",0,"
				  << totalPrimes << ","
				  << getElapsed(clockStart, clockFinish)
				  << std::endl;
	}

	delete[] v;

	if (PAPI_Reset(performanceValues) != PAPI_OK)
	{
		return MPI_Finalize();
	}

	if (PAPI_Destroy() == PAPI_OK)
	{
		return MPI_Finalize();
	}

	return 1;
}