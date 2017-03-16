#include "common.h"

void prettyPrint(const uint64_t idx, const uint64_t v)
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

void printPrimes(const std::vector<bool>& v)
{
	uint64_t rv = 0;
	uint64_t i = v.size() - 1;

	for (uint64_t i = 0; i < v.size(); i++)
	{
		if (v[i])
		{
			rv++;
		}
	}

	printf("#Primes: %" PRIu64 " (", rv);
	rv = MIN(rv, MAXIMUM_PRIMES);

	while (rv > 0)
	{
		if (v[i])
		{
			prettyPrint(rv--, i + 2);
		}

		i--;
	}
}

void printPrimes(const bool* v, const uint64_t sz)
{
	uint64_t rv = 0;
	uint64_t i = sz - 1;

	for (uint64_t i = 0; i < sz; i++)
	{
		if (v[i])
		{
			rv++;
		}
	}

	printf("#Primes: %" PRIu64 " (", rv);
	rv = MIN(rv, MAXIMUM_PRIMES);

	while (rv > 0)
	{
		if (v[i])
		{
			prettyPrint(rv--, i + 2);
		}

		i--;
	}
}

void printPrimes(const uint8_t* v, const uint64_t sz)
{
	uint64_t rv = 0;
	uint64_t i = sz - 1;

	for (uint64_t i = 0; i < sz; i++)
	{
		if (v[i] == 1)
		{
			rv++;
		}
	}

	printf("#Primes: %" PRIu64 " (", rv);
	rv = MIN(rv, MAXIMUM_PRIMES);

	while (rv > 0)
	{
		if (v[i] == 1)
		{
			prettyPrint(rv--, i + 2);
		}

		i--;
	}
}

double getElapsed(nanoTime start, nanoTime finish)
{
	return (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
}