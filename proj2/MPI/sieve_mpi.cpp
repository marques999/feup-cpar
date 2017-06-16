#include <mpi.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <inttypes.h>

#define MIN(a, b) (((a)<(b))?(a):(b))
#define BLOCK_LOW(i, n, p) ((i) * (n) / (p))
#define BLOCK_HIGH(i, n, p) (BLOCK_LOW((i) + 1, n, p) - 1)
#define BLOCK_SIZE(i, n, p) (BLOCK_LOW((i) + 1, n, p) - BLOCK_LOW(i, n, p))
#define BLOCK_OWNER(index, n, p) ((((p) * (index) + 1) - 1) / (n))

typedef struct timespec nanoTime;

static bool* initializeArray(uint64_t sz)
{
	bool* v = new bool[sz];

	if (v == NULL)
	{
		return NULL;
	}

	memset(v, false, sz);

	return v;
}

static double getElapsed(nanoTime start, nanoTime finish)
{
	return (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
}

void sieveSequential(uint64_t maximumPower)
{
	int clusterRank;
	int clusterSize;;
	double executionTime = 0.0;

	MPI_Comm_size(MPI_COMM_WORLD, &clusterSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &clusterRank);

	uint64_t startIndex;
	uint64_t localCount = 0;
	uint64_t primeCount = 1;
	uint64_t primeIndex = 0;
	uint64_t currentValue = 3;
	uint64_t maximumValue = pow(2, maximumPower);
	std::ofstream out("mpi-sequential.csv", std::ios::app);
	uint64_t blockSize = BLOCK_SIZE(clusterRank, maximumValue - 1, clusterSize);
	uint64_t lowerBound = 2 + BLOCK_LOW(clusterRank,  maximumValue - 1, clusterSize);
	uint64_t upperBound = 2 + BLOCK_HIGH(clusterRank, maximumValue - 1, clusterSize);
	uint64_t validateBlocks = 2 + (maximumValue - 1) / clusterSize;

	if (validateBlocks < (uint64_t)sqrt((double)maximumValue))
	{
		return;
	}

	if (lowerBound % 2 == 0)
	{
		if (upperBound % 2 == 0)
		{
			blockSize = (uint64_t)floor((double)blockSize / 2.0);
			upperBound--;
		}
		else
		{
			blockSize /= 2;
		}

		lowerBound++;
	}
	else if (upperBound % 2 == 0)
	{
		blockSize /= 2;
		upperBound--;
	}
	else
	{
		blockSize = (uint64_t)ceil((double)blockSize / 2.0);
	}

	bool* v = initializeArray(blockSize);

	if (v == NULL)
	{
		return;
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (clusterRank == 0)
	{
		startIndex = 0;
		out << clusterSize << ";" << maximumValue << ";" << maximumPower << ";";
		executionTime = -MPI_Wtime();
	}

	do
	{
		if (currentValue >= lowerBound)
		{
			startIndex = ((currentValue - lowerBound) / 2) + currentValue;
		}
		else if (currentValue * currentValue > lowerBound)
		{
			startIndex = (currentValue * currentValue - lowerBound) / 2;
		}
		else if (lowerBound % currentValue == 0)
		{
			startIndex = 0;
		}
		else
		{
			startIndex = 1;

			while ((lowerBound + (2 * startIndex)) % currentValue != 0)
			{
				++startIndex;
			}
		}

		for (uint64_t i = startIndex; i < blockSize; i += currentValue)
		{
			v[i] = true;
		}

		if (clusterRank == 0)
		{
			while (v[++primeIndex])
			{
			}

			currentValue = 3 + (primeIndex * 2);
		}

		MPI_Bcast(&currentValue, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	} while (currentValue * currentValue <= maximumValue);

	if (clusterRank == 0)
	{
		executionTime += MPI_Wtime();
		out << executionTime << ";";
	}

	for (uint64_t i = 0; i < blockSize; ++i)
	{
		if (v[i] == false)
		{
			++localCount;
		}
	}

	if (clusterSize > 1)
	{
		MPI_Reduce(&localCount, &primeCount, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	else
	{
		primeCount += localCount;
	}

	if (clusterRank == 0)
	{
		out << primeCount << std::endl;
		printf("#Primes: %" PRIu64 "\n", primeCount);
		printf("Elapsed: %.3f seconds\n", executionTime);
	}

	free(v);
	out.close();
}

void sieveParallel(uint64_t maximumPower, int numberThreads)
{
	int clusterRank;
	int clusterSize;;

	if (numberThreads < 1)
	{
		numberThreads = 1;
	}
	else if (numberThreads > omp_get_max_threads())
	{
		numberThreads = omp_get_max_threads();
	}

	omp_set_num_threads(numberThreads);
	MPI_Comm_size(MPI_COMM_WORLD, &clusterSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &clusterRank);

	nanoTime start;
	nanoTime finish;
	uint64_t startIndex;
	uint64_t localCount = 0;
	uint64_t primeCount = 1;
	uint64_t primeIndex = 0;
	uint64_t currentValue = 3;
	uint64_t maximumValue = pow(2, maximumPower);
	std::ofstream out("mpi-parallel.csv", std::ios::app);
	uint64_t blockSize = BLOCK_SIZE(clusterRank, maximumValue - 1, clusterSize);
	uint64_t lowerBound = 2 + BLOCK_LOW(clusterRank,  maximumValue - 1, clusterSize);
	uint64_t upperBound = 2 + BLOCK_HIGH(clusterRank, maximumValue - 1, clusterSize);
	uint64_t validateBlocks = 2 + (maximumValue - 1) / clusterSize;

	if (validateBlocks < (uint64_t)sqrt((double)maximumValue))
	{
		return;
	}

	if (lowerBound % 2 == 0)
	{
		if (upperBound % 2 == 0)
		{
			blockSize = (uint64_t)floor((double)blockSize / 2.0);
			upperBound--;
		}
		else
		{
			blockSize /= 2;
		}

		lowerBound++;
	}
	else if (upperBound % 2 == 0)
	{
		blockSize /= 2;
		upperBound--;
	}
	else
	{
		blockSize = (uint64_t)ceil((double)blockSize / 2.0);
	}

	bool* v = initializeArray(blockSize);

	if (v == NULL)
	{
		return;
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (clusterRank == 0)
	{
		startIndex = 0;
		out << clusterSize << ";" << maximumValue << ";" << maximumPower << ";";
		clock_gettime(CLOCK_MONOTONIC, &start);
	}

	do
	{
		if (currentValue >= lowerBound)
		{
			startIndex = ((currentValue - lowerBound) / 2) + currentValue;
		}
		else if (currentValue * currentValue > lowerBound)
		{
			startIndex = (currentValue * currentValue - lowerBound) / 2;
		}
		else if (lowerBound % currentValue == 0)
		{
			startIndex = 0;
		}
		else
		{
			startIndex = 1;

			while ((lowerBound + (2 * startIndex)) % currentValue != 0)
			{
				++startIndex;
			}
		}

		#pragma omp parallel for schedule(static)
		for (uint64_t i = startIndex; i < blockSize; i += currentValue)
		{
			v[i] = true;
		}

		if (clusterRank == 0)
		{
			while (v[++primeIndex])
			{
			}

			currentValue = 3 + (primeIndex * 2);
		}

		MPI_Bcast(&currentValue, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	} while (currentValue * currentValue <= maximumValue);

	double executionTime = 0.0;

	if (clusterRank == 0)
	{
		clock_gettime(CLOCK_MONOTONIC, &finish);
		executionTime = getElapsed(start, finish);
		out << executionTime << ";";
	}

	for (uint64_t i = 0; i < blockSize; ++i)
	{
		if (v[i] == false)
		{
			++localCount;
		}
	}

	if (clusterSize > 1)
	{
		MPI_Reduce(&localCount, &primeCount, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	else
	{
		primeCount += localCount;
	}

	if (clusterRank == 0)
	{
		out << primeCount << std::endl;
		printf("#Primes: %" PRIu64 "\n", primeCount);
		printf("Elapsed: %.3f seconds\n", executionTime);
	}

	free(v);
	out.close();
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	if (argc > 2)
	{
		sieveParallel((uint64_t) atol(argv[1]), (int) atoi(argv[2]));
	}
	else
	{
		sieveSequential((uint64_t) atol(argv[1]));
	}

	MPI_Finalize();

	return 0;
}
