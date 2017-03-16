#include <omp.h>
#include <time.h>
#include <cstdlib>
#include <iostream>

static double* initializeMatrixA(int mRows, int mColumns)
{
	double* m = (double*)malloc((mRows * mColumns) * sizeof(double));

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			m[i * mColumns + j] = (double) 1.0;
		}
	}

	return m;
}

static double* initializeMatrixB(int mRows, int mColumns)
{
	double* m = (double*)malloc((mRows * mColumns) * sizeof(double));

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			m[i * mColumns + j] = (double)(i + 1);
		}
	}

	return m;
}

static double getElapsed(clock_t clockStart, clock_t clockEnd)
{
	return ((double)(clockEnd - clockStart) / CLOCKS_PER_SEC);
}

static double getElapsed(double clockStart, double clockEnd)
{
	return clockEnd - clockStart;
}

static void printMatrix(double* m, int mDimension, double elapsedTime)
{
	char st[100];

	sprintf(st, "Time: %3.3f seconds", elapsedTime);

	std::cout << st << std::endl;

	for (int j = 0; j < std::min(10, mDimension); j++)
	{
		std::cout << m[j] << " ";
	}

	std::cout << std::endl;
}

void MultiplyNaive(int mRows, int mColumns)
{
	double* pha = initializeMatrixA(mRows, mColumns);
	double* phb = initializeMatrixB(mColumns, mRows);
	double* phc = (double*)malloc((mRows * mRows) * sizeof(double));

	clock_t clockStart = clock();

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mRows; j++)
		{
			double sumValue = 0;

			for (int k = 0; k < mColumns; k++)
			{
				sumValue += pha[i * mColumns + k] * phb[k * mRows + j];
			}

			phc[i * mRows + j] = sumValue;
		}
	}

	printMatrix(phc, mRows, getElapsed(clockStart, clock()));
	free(pha);
	free(phb);
	free(phc);
}

void MultiplyNaiveParallel(int mRows, int mColumns, int mThreads)
{
	double* pha = initializeMatrixA(mRows, mColumns);
	double* phb = initializeMatrixB(mColumns, mRows);
	double* phc = (double*)malloc((mRows * mRows) * sizeof(double));

	omp_set_num_threads(mThreads);

	double clockStart = omp_get_wtime();

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mRows; j++)
		{
			double sumValue = 0;

			#pragma omp parallel for reduction(+:sumValue)
			for (int k = 0; k < mColumns; k++)
			{
				sumValue += pha[i * mColumns + k] * phb[k * mRows + j];
			}

			phc[i * mRows + j] = sumValue;
		}
	}

	printMatrix(phc, mRows, getElapsed(clockStart, omp_get_wtime()));
	free(pha);
	free(phb);
	free(phc);
}

void MultiplyLine(int mRows, int mColumns)
{
	double* pha = initializeMatrixA(mRows, mColumns);
	double* phb = initializeMatrixB(mColumns, mRows);
	double* phc = (double*)malloc((mRows * mRows) * sizeof(double));

	clock_t clockStart = clock();

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			for (int k = 0; k < mRows; k++)
			{
				phc[i * mRows + k] += pha[i * mColumns + j] * phb[j * mRows + k];
			}
		}
	}

	printMatrix(phc, mRows, getElapsed(clockStart, clock()));
	free(pha);
	free(phb);
	free(phc);
}

void MultiplyLineParallel(int mRows, int mColumns, int mThreads)
{
	double* pha = initializeMatrixA(mRows, mColumns);
	double* phb = initializeMatrixB(mColumns, mRows);
	double* phc = (double*)malloc((mRows * mRows) * sizeof(double));
	double clockStart = omp_get_wtime();

	#pragma omp parallel for num_threads(mThreads)
	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			for (int k = 0; k < mRows; k++)
			{
				phc[i * mRows + k] += pha[i * mColumns + j] * phb[j * mRows + k];
			}
		}
	}

	printMatrix(phc, mRows, getElapsed(clockStart, omp_get_wtime()));
	free(pha);
	free(phb);
	free(phc);
}