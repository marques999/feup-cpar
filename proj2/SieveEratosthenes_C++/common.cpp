#include "common.h"

static uint64_t countPrimes(const bool* v, const uint64_t sz)
{
	uint64_t rv = 0;

	for (uint64_t i = 0; i < sz; i++)
	{
		if (v[i])
		{
			rv++;
		}
	}

	printf("#Primes: %" PRIu64 " (", rv);

	return MIN(rv, MAXIMUM_PRIMES);
}

static void prettyPrint(const uint64_t idx, const uint64_t v)
{
	if (idx == 1)
	{
		printf("%" PRIu64 ")\n", v);
	}
	else
	{
		printf("%" PRIu64 ", ", v);
	}
}

void printBitwise(const uint64_t* v, const uint64_t sz)
{
	uint64_t rv = 0;

	for (uint64_t j = 1; j <= sz; j++)
	{
		if (IS_PRIME(j))
		{
			++rv;
		}
	}

	printf("#Primes: %" PRIu64 " (", rv);
	rv = MIN(rv, MAXIMUM_PRIMES);

	for (uint64_t i = sz; rv > 0; i--)
	{
		if (IS_PRIME(i))
		{
			prettyPrint(rv--, i);
		}
	}
}

void printEven(const bool* v, const uint64_t sz)
{
	uint64_t rv = countPrimes(v, sz);

	for (uint64_t i = sz - 1; rv > 0; i--)
	{
		if (v[i])
		{
			prettyPrint(rv--, i == 0 ? 2 : 1 + i * 2);
		}
	}
}

void printPrimes(const bool* v, const uint64_t sz)
{
	uint64_t rv = countPrimes(v, sz);

	for (uint64_t i = sz - 1; rv > 0; i--)
	{
		if (v[i])
		{
			prettyPrint(rv--, i + 2);
		}
	}
}

double getElapsed(nanoTime start, nanoTime finish)
{
	return (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
}
