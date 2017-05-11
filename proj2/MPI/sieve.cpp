#include "common.h"

void sieveSequential(uint64_t maximumPower, ofstream& out)
{
	int clusterRank;
	int clusterSize;
	double executionTime = 0;

	MPI_Comm_size(MPI_COMM_WORLD, &clusterSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &clusterRank);

	uint64_t startIndex;
	uint64_t maximumValue = pow(2, maximumPower);
	uint64_t blockSize = BLOCK_SIZE(clusterRank, maximumValue - 1, clusterSize);
	uint64_t lowerIndex = BLOCK_LOW(clusterRank, maximumValue - 1, clusterSize) + 2;
	uint64_t upperIndex = BLOCK_HIGH(clusterRank, maximumValue - 1, clusterSize) + 2;

	bool* v = newList(blockSize);

	MPI_Barrier(MPI_COMM_WORLD);

	if (clusterRank == 0)
	{
		out << clusterSize << ";" << maximumValue << ";" << maximumPower << ";";
		executionTime = -MPI_Wtime();
	}

	uint64_t currentValue = 2;
	uint64_t localCount = 0;
	uint64_t numberPrimes = 0;

	while (currentValue * currentValue <= maximumValue)
	{
		if ((currentValue * currentValue) < lowerIndex)
		{
			if (lowerIndex % currentValue == 0)
			{
				startIndex = lowerIndex;
			}
			else
			{
				startIndex = lowerIndex + (currentValue - (lowerIndex % currentValue));
			}
		}
		else
		{
			startIndex = currentValue * currentValue;
		}

		for (uint64_t i = startIndex; i <= upperIndex; i += currentValue)
		{
			v[i - lowerIndex] = false;
		}

		if (clusterRank == 0)
		{
			for (uint64_t i = currentValue + 1; i < upperIndex; i++)
			{
				if (v[i - lowerIndex])
				{
					currentValue = i;
					break;
				}
			}
		}

		MPI_Bcast(&currentValue, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	}

	if (clusterRank == 0)
	{
		executionTime += MPI_Wtime();
		out << executionTime << ";";
	}

	for (uint64_t i = 0; i < blockSize; i++)
	{
		if (v[i])
		{
			localCount++;
		}
	}

	if (clusterSize > 1)
	{
		MPI_Reduce(&localCount, &numberPrimes, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	else
	{
		numberPrimes = localCount;
	}

	if (clusterRank == 0)
	{
		out << numberPrimes << endl;
	}

	free(v);
}

void sieveParallel(uint64_t maximumPower, int numberThreads, ofstream& out)
{
	int clusterRank, clusterSize;
	double executionTime = 0;

	uint64_t startIndex, localCount = 0, primeCount = 0, maximumValue = pow(2, maximumPower);

	if (numberThreads > omp_get_max_threads())
	{
		numberThreads = omp_get_max_threads();
	}

	MPI_Comm_size(MPI_COMM_WORLD, &clusterSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &clusterRank);

	uint64_t blockSize = BLOCK_SIZE(clusterRank, maximumValue - 1, clusterSize);
	uint64_t lowerIndex = BLOCK_LOW(clusterRank, maximumValue - 1, clusterSize) + 2;
	uint64_t upperIndex = BLOCK_HIGH(clusterRank, maximumValue - 1, clusterSize) + 2;

	bool* v = newList(blockSize);

	MPI_Barrier(MPI_COMM_WORLD);

	if (clusterRank == 0)
	{
		out << clusterSize << ";" << maximumValue << ";" << maximumPower << ";" << numberThreads << ";";
		executionTime = -MPI_Wtime();
	}

	uint64_t currentValue = 2;

	while ((currentValue * currentValue) <= maximumValue)
	{
		if ((currentValue * currentValue) < lowerIndex)
		{
			if (lowerIndex % currentValue == 0)
			{
				startIndex = lowerIndex;
			}
			else
			{
				startIndex = lowerIndex + (currentValue - (lowerIndex % currentValue));
			}
		}
		else
		{
			startIndex = currentValue * currentValue;
		}

		#pragma omp parallel for num_threads(threads)
		for (uint64_t i = startIndex; i <= upperIndex; i += currentValue)
		{
			v[i - lowerIndex] = false;
		}

		if (clusterRank == 0)
		{
			for (uint64_t i = currentValue + 1; i < upperIndex; i++)
			{
				if (v[i - lowerIndex])
				{
					currentValue = i;
					break;
				}
			}
		}

		MPI_Bcast(&currentValue, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	}

	if (clusterRank == 0)
	{
		executionTime += MPI_Wtime();
		out << executionTime << ";";
	}

	for (uint64_t i = 0; i < blockSize; i++)
	{
		if (v[i])
		{
			localCount++;
		}
	}

	if (clusterSize > 1)
	{
		MPI_Reduce(&localCount, &primeCount, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	else
	{
		primeCount = localCount;
	}

	if (clusterRank == 0)
	{
		out << primeCount << endl;
	}

	free(v);
}

int main(int argc, char** argv)
{
	ofstream out;
	uint64_t maximumPower = (uint64_t) atol(argv[1]);

	MPI_Init(&argc, &argv);

	if (argc > 2)
	{
		out.open("mpi-parallel.csv", ios::app);
		sieveParallel(maximumPower, (int) atoi(argv[2]), out);
	}
	else
	{
		out.open("mpi-sequential.csv", ios::app);
		sieveSequential(maximumPower, out);
	}

	MPI_Finalize();
	out.close();

	return 0;
}
