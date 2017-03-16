#include "common.h"

static double runDefault(const uint64_t maximumValue, int maximumThreads)
{
	bool* v = new bool[maximumValue - 1];

	for (uint64_t i = 0; i < maximumValue - 1; i++)
	{
		v[i] = true;
	}

	nanoTime start;
	nanoTime finish;
	uint64_t k = 2;

	clock_gettime(CLOCK_MONOTONIC, &start);

	while (k * k <= maximumValue)
	{
		#pragma omp parallel for num_threads(maximumThreads)
		for (uint64_t i = k * k; i <= maximumValue; i += k)
		{
			v[i - 2] = false;
		}

		for (uint64_t i = k + 1; i <= maximumValue; i++)
		{
			if (v[i - 2])
			{
				k = i;
				break;
			}
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printPrimes(v, maximumValue - 1);

	return getElapsed(start, finish);
}

static const ParallelFunction functionPointers[NUMBER_ALGORITHMS] =
{
	runDefault,
	runDefault,
	runDefault,
	runDefault
};

void RunParallel(const uint64_t maximumValue, int improvementIndex, int maximumThreads)
{
	if (improvementIndex < NUMBER_ALGORITHMS)
	{
		printf("%.3f\n", functionPointers[improvementIndex](maximumValue, maximumThreads));
	}
}