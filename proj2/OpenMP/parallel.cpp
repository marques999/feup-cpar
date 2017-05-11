#include <omp.h>
#include "papi.h"
#include "common.h"

static double OMP_SkipEven(const uint64_t maximumValue, int numberThreads)
{
	uint64_t i;
	uint64_t j;
	nanoTime start;
	nanoTime finish;
	uint64_t arrayLength = (maximumValue - 1) / 2;
	uint64_t maximumSqrt = (int)sqrt((double)maximumValue);

	bool* v = new bool[arrayLength + 1];

	memset(v, true, arrayLength + 1);

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

	long long values[3];

	clock_gettime(CLOCK_MONOTONIC, &start);

	#pragma omp parallel for schedule(dynamic)
	for (i = 3; i <= maximumSqrt; i += 2)
	{
		if (v[i / 2])
		{
			for (j = i * i; j <= maximumValue; j += 2 * i)
			{
				v[j / 2] = false;
			}
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

	printEven(v, arrayLength + 1);

	return getElapsed(start, finish);
}

static double OMP_FastMarking(const uint64_t maximumValue, int numberThreads)
{
	nanoTime start;
	nanoTime finish;
	uint64_t primeCount = 1;
	uint64_t currentValue = 3;
	uint64_t sqrtMaximum = ceil(sqrt((double)maximumValue));
	uint64_t validateThreads = 1 + ((maximumValue - 1) / numberThreads);

	if (validateThreads < (uint64_t)sqrt((double)maximumValue))
	{
		return 0.0;
	}

	bool* preMarked = new bool[sqrtMaximum + 1];

	memset(preMarked, false, sqrtMaximum + 1);
	preMarked[0] = true;
	preMarked[1] = true;

	uint64_t kIndex;
	uint64_t preK = 2;

	do
	{
		for (uint64_t i = preK * preK; i <= sqrtMaximum; i += preK)
		{
			preMarked[i] = true;
		}

		while (preMarked[++preK])
		{
		}
	} while (preK * preK <= sqrtMaximum);

	std::vector<uint64_t> kset;

	for (uint64_t i = 3; i <= sqrtMaximum; ++i)
	{
		if (preMarked[i] == false)
		{
			kset.push_back(i);
		}
	}

	free(preMarked);

	if (kset.empty())
	{
		return 0.0;
	}

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

	int threadId;
	long long values[3];

	clock_gettime(CLOCK_MONOTONIC, &start);

	#pragma omp parallel for default(shared) private(threadId, kIndex, currentValue)
	for (threadId = 0; threadId < numberThreads; ++threadId)
	{
		kIndex = 0;
		currentValue = kset[kIndex];

		uint64_t firstValue;
		uint64_t lowerBound = 2 + BLOCK_LOW(threadId, numberThreads, maximumValue - 1);
		uint64_t upperBound = 2+ BLOCK_HIGH(threadId, numberThreads, maximumValue - 1);
		uint64_t blockLength = BLOCK_SIZE(threadId, numberThreads, maximumValue - 1);

		if (lowerBound % 2 == 0)
		{
			if (upperBound % 2 == 0)
			{
				blockLength = (uint64_t)floor((double)blockLength / 2.0);
				upperBound--;
			}
			else
			{
				blockLength /= 2;
			}

			lowerBound++;
		}
		else if (upperBound % 2 == 0)
		{
			blockLength /= 2;
			upperBound--;
		}
		else
		{
			blockLength = (uint64_t)ceil((double)blockLength / 2.0);
		}

		bool* v = new bool[blockLength];

		memset(v, false, blockLength);

		do
		{
			if (currentValue >= lowerBound)
			{
				firstValue = ((currentValue - lowerBound) / 2) + currentValue;
			}
			else if (currentValue * currentValue > lowerBound)
			{
				firstValue = (currentValue * currentValue - lowerBound) / 2;
			}
			else if (lowerBound % currentValue == 0)
			{
				firstValue = 0;
			}
			else
			{
				firstValue = 1;

				while ((lowerBound + (2 * firstValue)) % currentValue != 0)
				{
					++firstValue;
				}
			}

			for (uint64_t i = firstValue; i < blockLength; i += currentValue)
			{
				v[i] = true;
			}

			currentValue = kset[++kIndex];
		} while (currentValue * currentValue <= maximumValue && kIndex < kset.size());

		uint64_t localCount = 0;

		for (uint64_t i = 0; i < blockLength; ++i)
		{
			if (v[i] == false)
			{
				++localCount;
			}
		}

		#pragma omp atomic
		primeCount += localCount;
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

	std::cout << primeCount << " primes found between 2 and " << maximumValue << std::endl;

	return getElapsed(start, finish);
}

static const ParallelFunction functionPointers[NUMBER_ALGORITHMS] =
{
	OMP_SkipEven,
	OMP_FastMarking
};

void RunParallel(int algorithmIndex, uint64_t maximumValue, int numberThreads)
{
	if (numberThreads < 1)
	{
		numberThreads = 1;
	}
	else if (numberThreads > omp_get_max_threads())
	{
		numberThreads = omp_get_max_threads();
	}

	omp_set_num_threads(numberThreads);

	if (algorithmIndex < NUMBER_ALGORITHMS_OMP)
	{
		printf("Elapsed: %.3f seconds\n", functionPointers[algorithmIndex](maximumValue, numberThreads));
	}
}
