#include "common.h"

static double runBoolVector(const uint64_t maximumValue)
{
	nanoTime start;
	nanoTime finish;
	uint64_t current = 2;
	uint64_t smallest = 3;
	std::vector<bool> v(maximumValue - 1, true);

	clock_gettime(CLOCK_MONOTONIC, &start);

	while (current * current <= maximumValue)
	{
		for (uint64_t i = current * current; i <= maximumValue; i += current)
		{
			v[i - 2] = false;
		}

		for (uint64_t i = current + 1; i <= maximumValue; i++)
		{
			if (v[i - 2])
			{
				smallest = i;
				break;
			}
		}

		current = smallest;
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printPrimes(v);

	return getElapsed(start, finish);
}

static double runBoolArray(const uint64_t maximumValue)
{
	bool* v = new bool[maximumValue - 1];

	for (uint64_t i = 0; i < maximumValue - 1; i++)
	{
		v[i] = true;
	}

	nanoTime start;
	nanoTime finish;
	uint64_t k = 2;
	uint64_t smallest = 3;

	clock_gettime(CLOCK_MONOTONIC, &start);

	while (k * k <= maximumValue)
	{
		for (uint64_t i = k * k; i <= maximumValue; i += k)
		{
			v[i - 2] = false;
		}

		for (uint64_t i = k + 1; i <= maximumValue; i++)
		{
			if (v[i - 2])
			{
				smallest = i;
				break;
			}
		}

		k = smallest;
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printPrimes(v, maximumValue - 1);

	return getElapsed(start, finish);
}

static double runUnsignedChar(const uint64_t maximumValue)
{
	nanoTime start;
	nanoTime finish;
	uint8_t* v = new uint8_t[maximumValue - 1];

	for (uint64_t i = 0; i < maximumValue - 1; i++)
	{
		v[i - 2] = 1;
	}

	uint64_t current = 2;
	uint64_t smallest = 3;

	clock_gettime(CLOCK_MONOTONIC, &start);

	while (current * current <= maximumValue)
	{
		for (uint64_t i = current * current; i <= maximumValue; i += current)
		{
			v[i - 2] = false;
		}

		for (uint64_t i = current + 1; i <= maximumValue; i++)
		{
			if (v[i - 2])
			{
				smallest = i;
				break;
			}
		}

		current = smallest;
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printPrimes(v, maximumValue - 1);

	return getElapsed(start, finish);
}

static double runNoSmallest(const uint64_t maximumValue)
{
	nanoTime start;
	nanoTime finish;
	std::vector<bool> v(maximumValue - 1, true);

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (uint64_t i = 2; i * i <= maximumValue; i++)
	{
		if (v[i - 2])
		{
			for (uint64_t j = i * i; j <= maximumValue - 1; j += i)
			{
				v[j - 2] = false;
			}
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printPrimes(v);

	return getElapsed(start, finish);
}

static const SequentialFunction functionPointers[NUMBER_ALGORITHMS] =
{
	runBoolVector,
	runUnsignedChar,
	runNoSmallest,
	runBoolArray
};

void RunSequential(int improvementIndex, const uint64_t maximumValue)
{
	if (improvementIndex < NUMBER_ALGORITHMS)
	{
		printf("Elapsed: %.3f seconds\n", functionPointers[improvementIndex](maximumValue));
	}
}