#include <omp.h>

#include "papi.h"
#include "common.h"

static double runStatic(const uint64_t maximumValue, int numberThreads)
{
	const uint64_t arrayLength = (maximumValue - 1) / 2;
	bool* v = new bool[arrayLength + 1];

	for (uint64_t k = 0; k <= arrayLength; ++k)
	{
		v[k] = true;
	}

	uint64_t i;
	uint64_t j;
	nanoTime start;
	nanoTime finish;
	uint64_t maximumSqrt = (int)sqrt((double)maximumValue);

	omp_set_num_threads(numberThreads);
	clock_gettime(CLOCK_MONOTONIC, &start);

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

	#pragma omp parallel for schedule(static)
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

	long long values[3];

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printEven(v, arrayLength + 1);

	return getElapsed(start, finish);
}

static double runDynamic(const uint64_t maximumValue, int numberThreads)
{
	const uint64_t arrayLength = (maximumValue - 1) / 2;
	bool* v = new bool[arrayLength + 1];

	for (uint64_t k = 0; k <= arrayLength; ++k)
	{
		v[k] = true;
	}

	uint64_t i;
	uint64_t j;
	nanoTime start;
	nanoTime finish;
	uint64_t maximumSqrt = (int)sqrt((double)maximumValue);

	omp_set_num_threads(numberThreads);
	clock_gettime(CLOCK_MONOTONIC, &start);

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

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

	long long values[3];

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	printEven(v, arrayLength + 1);

	return getElapsed(start, finish);
}

static double runFastMarking(const uint64_t maximumValue, int numberThreads)
{
	int threadId = 0;

	if (numberThreads > omp_get_max_threads())
	{
		numberThreads = omp_get_max_threads();
	}

	uint64_t k = 2;
	nanoTime start;
	nanoTime finish;
	uint64_t count = 0;
	uint64_t primeIndex = 0;
	uint64_t sqrtMaximum = (uint64_t) sqrt((double)maximumValue);
	uint64_t validateThreads = 1 + ((maximumValue - 1) / numberThreads);

	omp_set_num_threads(numberThreads);
	clock_gettime(CLOCK_MONOTONIC, &start);

	if (PAPI_Begin() != PAPI_OK)
	{
		return 0.0;
	}

	bool* v = new bool[maximumValue];

	memset(v, true, maximumValue);

	uint64_t first = 0;
	uint64_t low = 2 + BLOCK_LOW(threadId, numberThreads, maximumValue);
	uint64_t high = 2 + BLOCK_HIGH(threadId, numberThreads, maximumValue);
	uint64_t length = BLOCK_SIZE(threadId, numberThreads, maximumValue);

	while(k * k <= maximumValue){
			if(k * k < low){
				if(low % k == 0){
					first = low;
				}else{
					first = low + (k - (low % k));
				}
			}else{
				first = k * k;
			}

			#pragma omp parallel for num_threads(numberThreads)
			for(uint64_t i = first; i <= low + length ; i += k){
				std::cout << "HEY";
				v[i - low] = false;
			}

			std::cout << k;

			if(true){
				for(uint64_t i = k + 1 ; i < low + length ; i++){
					if(v[i - low]){
						k = i;
						break;
					}
				}
			}
	}
	/*#pragma omp parallel for if(validateThreads >= sqrtMaximum) default(shared) private(threadId)
	for (threadId = 0; threadId < numberThreads; threadId++)
	{
		uint64_t first = 0;
		uint64_t low = 2 + BLOCK_LOW(threadId, numberThreads, maximumValue);
		uint64_t length = BLOCK_SIZE(threadId, numberThreads, maximumValue);

		do
		{
			if (k > low)
			{
				first = k - low + k;
			}
			else if (k * k > low)
			{
				first = k * k - low;
			}
			else if (low % k == 0)
			{
				first = 0;
			}
			else
			{
				first = k - (low % k);
			}

			for (uint64_t i = first; i <= length; i += k)
			{
				v[i] = false;
			}

			if (threadId == 0)
			{
				while (v[++primeIndex] == false);
				k = primeIndex + 2;
			}

		} while (k * k <= maximumValue);

		uint64_t lcount = 0;

		for (uint64_t i= 0; i <= length; i++)
		{
			if (v[i])
			{
				lcount++;
			}
		}

		free(v);
		v = NULL;

		#pragma omp atomic
		count += lcount;
	}*/

	uint64_t lcount = 0;

	for (uint64_t i= 0; i <= maximumValue; i++)
	{
		if (v[i])
		{
			lcount++;
		}
	}

	long long values[3];

	if (PAPI_Reset(values) != PAPI_OK)
	{
		return 0.0;
	}

    clock_gettime(CLOCK_MONOTONIC, &finish);
    std::cout << lcount << " primes found between 2 and " << maximumValue << std::endl;

    return getElapsed(start, finish);
}

static const ParallelFunction functionPointers[NUMBER_ALGORITHMS] =
{
	runStatic,
	runDynamic,
	runFastMarking
};

void RunParallel(int algorithmIndex, uint64_t maximumValue, int numberThreads)
{
	if (algorithmIndex < NUMBER_ALGORITHMS_OMP)
	{
		printf("Elapsed: %.3f seconds\n", functionPointers[algorithmIndex](maximumValue, numberThreads));
	}
}
