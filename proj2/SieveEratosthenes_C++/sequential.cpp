#include "common.h"

static double runBitwise(const uint64_t maximumValue)
{
	nanoTime start;
	nanoTime finish;
	uint64_t sqrtMaximum = sqrt(maximumValue) + 1;
	uint64_t memorySize = (maximumValue >> 5);

	if (memorySize == 0)
	{
		memorySize = 1;
	}

	uint64_t* v = new uint64_t[memorySize];

	if (maximumValue < 32)
	{
		v[0] = (1 << maximumValue) - 1;
	}
	else
	{
		for (uint64_t i = 0; i < memorySize; ++i)
		{
			v[i] = 0xffffffff;
		}
	}

	v[0] &= ~(1 << 1);
	clock_gettime(CLOCK_MONOTONIC, &start);

	for (uint64_t i = 2; i <= sqrtMaximum; ++i)
	{
		if (v[i >> 5] & (1 << (i & 31)))
		{
			for (uint64_t j = i << 1; j < maximumValue; j += i)
			{
				v[j >> 5] &= ~(1 << (j & 31));
			}
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printBitwise(v, maximumValue);

	return getElapsed(start, finish);
}

static double runBoolArray(const uint64_t maximumValue)
{
	bool* v = new bool[maximumValue - 1];

	for (uint64_t i = 0; i < maximumValue - 1; ++i)
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

		for (uint64_t i = k + 1; i <= maximumValue; ++i)
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

static double runExcludeOdd(const uint64_t maximumValue)
{
	const uint64_t memorySize = (maximumValue - 1) / 2;
	bool* v = new bool[memorySize + 1];

	for (uint64_t i = 0; i <= memorySize; ++i)
	{
		v[i] = true;
	}

	nanoTime start;
	nanoTime finish;

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (uint64_t i = 3; i * i <= maximumValue; i += 2)
	{
		if (v[i / 2])
		{
			for (uint64_t j = i * i; j <= maximumValue; j += 2 * i)
			{
				v[j / 2] = false;
			}
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printEven(v, memorySize + 1);

	return getElapsed(start, finish);
}

static double runFastMarking(const uint64_t maximumValue)
{
	uint64_t k = 2;
	nanoTime start;
	nanoTime finish;
	uint64_t lowValue = 2;
	uint64_t primeIndex = 0;
	uint64_t firstIndex = 0;

	bool* v = new bool[maximumValue + 1];

	for (uint64_t i = 0; i <= maximumValue; ++i)
	{
		v[i] = true;
	}

	clock_gettime(CLOCK_MONOTONIC, &start);

	do
	{
		if (k > lowValue)
		{
			firstIndex = k - lowValue + k;
		}
		else if (k * k > lowValue)
		{
			firstIndex = k * k - lowValue;
		}
		else if (lowValue % k == 0)
		{
			firstIndex = 0;
		}
		else
		{
			firstIndex = k - (lowValue % k);
		}

		for (uint64_t i = firstIndex; i <= maximumValue; i += k)
		{
			v[i] = false;
		}

		while (v[++primeIndex] == false)
		{
		}

		k = primeIndex + 2;
	}
	while (k * k <= maximumValue);

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printPrimes(v, maximumValue);

	return getElapsed(start, finish);
}

static const SequentialFunction functionPointers[NUMBER_ALGORITHMS] =
{
	runBitwise,
	runBoolArray,
	runExcludeOdd,
	runFastMarking
};

void RunSequential(int algorithmIndex, const uint64_t maximumValue)
{
	if (algorithmIndex < NUMBER_ALGORITHMS)
	{
		printf("Elapsed: %.3f seconds\n", functionPointers[algorithmIndex](maximumValue));
	}
}
