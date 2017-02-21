#include <omp.h>
#include <papi.h>
#include <iostream>

#include "matrix.h"
#include "papi.h"

int askThreads()
{
	int numberThreads;
	int maximumThreads = omp_get_num_procs();

	std::cout << "Working threads " << "(maximum " << maximumThreads << ") ? ";
	std::cin >> numberThreads;

	if (numberThreads < 1)
	{
		numberThreads = 1;
	}
	else if (numberThreads > maximumThreads)
	{
		numberThreads = maximumThreads;
	}

	return numberThreads;
}

static void runBenchmark(int start, int end, int step, void(*matrixOperation)(int, int))
{
	for (int i = start; i <= end; i += step)
	{
		std::cout << "[" << i << "x" << i << "]\n";
		matrixOperation(i, i);
	}
}

static void runBenchmarkOMP(int numberThreads, void(*matrixOperation)(int, int, int))
{
	for (int i = 600; i <= 3000; i += 400)
	{
		std::cout << "[" << i << "x" << i << "]\n";
		matrixOperation(i, i, numberThreads);
	}
}

static void BenchmarkNaive()
{
	std::cout << "Running single-threaded naive multiplication benchmark...\n\n";
	runBenchmark(600, 3000, 400, MultiplyNaive);
}

static void BenchmarkLine()
{
	std::cout << "Running single-threaded line multiplication benchmark...\n\n";
	runBenchmark(600, 3000, 400, MultiplyLine);
	runBenchmark(4000, 10000, 2000, MultiplyLine);
}

static void BenchmarkNaiveParallel(int numberThreads)
{
	std::cout << "Running multi-threaded naive multiplication benchmark (" << numberThreads << " threads)...\n\n";
	runBenchmarkOMP(numberThreads, MultiplyNaiveParallel);
}

static void BenchmarkLineParallel(int numberThreads)
{
	std::cout << "Running multi-threaded line multiplication benchmark (" << numberThreads << " threads)...\n\n";
	runBenchmarkOMP(numberThreads, MultiplyLineParallel);
}

int main(int argc, char* argv[])
{
	int mRows;
	int mColumns;
	long long values[2];

	if (PAPI_Initialize() != PAPI_OK)
	{
		return 1;
	}

	int op = 1;

	do
	{
		std::cout << "+---------------------------------------+\n";
		std::cout << "| 1. Multiplication                     |\n";
		std::cout << "| 2. Line Multiplication                |\n";
		std::cout << "| 3. Parallel Multiplication            |\n";
		std::cout << "| 4. Parallel Line Multiplication       |\n";
		std::cout << "+---------------------------------------+\n";
		std::cout << "| 5. Benchmark [Sequential Naive]       |\n";
		std::cout << "| 6. Benchmark [Sequential Line]        |\n";
		std::cout << "| 7. Benchmark [Parallel Naive]         |\n";
		std::cout << "| 8. Benchmark [Parallel Line]          |\n";
		std::cout << "+---------------------------------------+\n";
		std::cout << "| 0. Exit                               |\n";
		std::cout << "+---------------------------------------+\n\n";
		std::cout << "Selection ? ";
		std::cin >> op;

		if (op <= 0)
		{
			break;
		}

		if (op <= 4)
		{
			std::cout << "Dimensions: lins cols ? ";
			std::cin >> mRows >> mColumns;
		}

		if (PAPI_Begin() != PAPI_OK)
		{
			return 1;
		}

		switch (op)
		{
		case 1:
			MultiplyNaive(mRows, mColumns);
			break;
		case 2:
			MultiplyLine(mRows, mColumns);
			break;
		case 3:
			MultiplyNaiveParallel(mRows, mColumns, askThreads());
			break;
		case 4:
			MultiplyLineParallel(mRows, mColumns, askThreads());
			break;
		case 5:
			BenchmarkNaive();
			break;
		case 6:
			BenchmarkLine();
			break;
		case 7:
			BenchmarkNaiveParallel(askThreads());
			break;
		case 8:
			BenchmarkLineParallel(askThreads());
			break;
		}

		if (PAPI_Reset(values) != PAPI_OK)
		{
			return 1;
		}
	} while (op != 0);

	if (PAPI_Destroy() != PAPI_OK)
	{
		return 1;
	}

	return 0;
}