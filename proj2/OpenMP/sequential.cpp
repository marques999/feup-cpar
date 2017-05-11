#include "common.h"
#include "papi.h"

static double Sequential_Bitwise(const uint64_t maximumValue)
{
	nanoTime start;
	nanoTime finish;
	uint64_t sqrtMaximum = sqrt(maximumValue) + 1;
	uint64_t memorySize = (maximumValue >> 5);

	if (memorySize == 0)
	{
		memorySize = 1;
	}

	uint32_t* v = new uint32_t[memorySize];

	if (maximumValue < 32)
	{
		v[0] = (1 << maximumValue) - 1;
	}
	else
	{
		memset(v, ~0, memorySize * sizeof(v[0]));
	}

	v[0] &= ~(1 << 1);

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

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

	long long values[3];

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

	printBitwise(v, maximumValue);

	return getElapsed(start, finish);
}

static double Sequential_BoolArray(const uint64_t maximumValue)
{
	uint64_t k = 2;
	nanoTime start;
	nanoTime finish;
	uint64_t primeIndex = 3;

	bool* v = new bool[maximumValue - 1];

	memset(v, true, maximumValue - 1);

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

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
				primeIndex = i;
				break;
			}
		}

		k = primeIndex;
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);

	long long values[3];

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

	printPrimes(v, maximumValue - 1);

	return getElapsed(start, finish);
}

static double Sequential_SkipEven(const uint64_t maximumValue)
{
	nanoTime start;
	nanoTime finish;
	uint64_t memorySize = (maximumValue - 1) / 2;

	bool* v = new bool[memorySize + 1];

	memset(v, true, memorySize + 1);

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

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

	long long values[3];

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

	printEven(v, memorySize + 1);

	return getElapsed(start, finish);
}

static double Sequential_FastMarking(const uint64_t maximumValue)
{
	uint64_t k = 2;
	nanoTime start;
	nanoTime finish;
	uint64_t lowerBound = 2;
	uint64_t primeIndex = 0;
	uint64_t firstValue = 0;

	bool* v = new bool[maximumValue + 1];

	memset(v, true, maximumValue + 1);

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

	clock_gettime(CLOCK_MONOTONIC, &start);

	do
	{
		if (k > lowerBound)
		{
			firstValue = k - lowerBound + k;
		}
		else if (k * k > lowerBound)
		{
			firstValue = k * k - lowerBound;
		}
		else if (lowerBound % k == 0)
		{
			firstValue = 0;
		}
		else
		{
			firstValue = k - (lowerBound % k);
		}

		for (uint64_t i = firstValue; i <= maximumValue; i += k)
		{
			v[i] = false;
		}

		while (v[++primeIndex] == false)
		{
		}

		k = primeIndex + 2;
	} while (k * k <= maximumValue);

	clock_gettime(CLOCK_MONOTONIC, &finish);

	long long values[3];

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

	printPrimes(v, maximumValue);

	return getElapsed(start, finish);
}

static const SequentialFunction functionPointers[NUMBER_ALGORITHMS] =
{
	Sequential_Bitwise,
	Sequential_BoolArray,
	Sequential_SkipEven,
	Sequential_FastMarking
};

void RunSequential(int algorithmIndex, const uint64_t maximumValue)
{
	if (algorithmIndex < NUMBER_ALGORITHMS)
	{
		printf("Elapsed: %.3f seconds\n", functionPointers[algorithmIndex](maximumValue));
	}
}
