package cpar_proj1;

import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class MatrixOperations
{
	private static double[][] initializeMatrixA(int mRows, int mColumns)
	{
		double[][] m = new double[mRows][mColumns];

		for (int i = 0; i < mRows; i++)
		{
			for (int j = 0; j < mColumns; j++)
			{
				m[i][j] = 1.0;
			}
		}

		return m;
	}

	private static double[][] initializeMatrixB(int mRows, int mColumns)
	{
		double[][] m = new double[mRows][mColumns];

		for (int i = 0; i < mRows; i++)
		{
			for (int j = 0; j < mColumns; j++)
			{
				m[i][j] = i + 1;
			}
		}

		return m;
	}

	private static void PrintMatrix(double[][] matrix, int sz, long elapsedTime)
	{
		System.out.format("Time: %3.3f seconds\n", elapsedTime / 1000.0);

		for (int j = 0; j < Math.min(10, sz); j++)
		{
			System.out.print(matrix[0][j] + " ");
		}

		System.out.println("\n");
	}

	public static void Multiply(int mRows, int mColumns)
	{
		double[][] pha = initializeMatrixA(mRows, mColumns);
		double[][] phb = initializeMatrixB(mColumns, mRows);
		double[][] phc = new double[mRows][mRows];

		long start = System.currentTimeMillis();

		for (int i = 0; i < mRows; i++)
		{
			for (int j = 0; j < mRows; j++)
			{
				double sumValue = 0;

				for (int k = 0; k < mColumns; k++)
				{
					sumValue += pha[i][k] * phb[k][j];
				}

				phc[i][j] = sumValue;
			}
		}

		PrintMatrix(phc, mRows, System.currentTimeMillis() - start);
	}

	public static void MultiplyLine(int mRows, int mColumns)
	{
		double[][] pha = initializeMatrixA(mRows, mColumns);
		double[][] phb = initializeMatrixB(mColumns, mRows);
		double[][] phc = new double[mRows][mRows];

		long start = System.currentTimeMillis();

		for (int i = 0; i < mRows; i++)
		{
			for (int j = 0; j < mColumns; j++)
			{
				for (int k = 0; k < mRows; k++)
				{
					phc[i][k] += pha[i][j] * phb[j][k];
				}
			}
		}

		PrintMatrix(phc, mRows, System.currentTimeMillis() - start);
	}

	private static int poolSize = Runtime.getRuntime().availableProcessors();

	public static void MultiplyParallel(int mRows, int mColumns)
	{
		double[][] pha = initializeMatrixA(mRows, mColumns);
		double[][] phb = initializeMatrixB(mColumns, mRows);
		double[][] phc = new double[mRows][mRows];

		final ExecutorService executor = Executors.newFixedThreadPool(poolSize);
		final List<Future<double[][]>> list = new ArrayList<Future<double[][]>>();
		int part = pha.length / poolSize;

		if (part < 1)
		{
			part = 1;
		}

		long beginTime = System.currentTimeMillis();

		for (int i = 0; i < pha.length; i += part)
		{
			list.add(executor.submit(new DefaultMultiplier(pha, phb, mRows, mColumns, i, i + part)));
		}

		int start = 0;
		double CF[][];

		for (final Future<double[][]> future : list)
		{
			try
			{
				CF = future.get();

				for (int i = start; i < start + part; i += 1)
				{
					phc[i] = CF[i];
				}
			}
			catch (final Exception ex)
			{
				ex.printStackTrace();
			}

			start += part;
		}

		executor.shutdown();
		PrintMatrix(phc, mRows, System.currentTimeMillis() - beginTime);
	}

	public static void MultiplyLineParallel(int mRows, int mColumns)
	{
		double[][] CF;
		double[][] pha = initializeMatrixA(mRows, mColumns);
		double[][] phb = initializeMatrixB(mColumns, mRows);
		double[][] phc = new double[mRows][mRows];
		final ExecutorService executor = Executors.newFixedThreadPool(poolSize);
		final List<Future<double[][]>> list = new ArrayList<Future<double[][]>>();
		int part = pha.length / poolSize;

		if (part < 1)
		{
			part = 1;
		}

		long beginTime = System.currentTimeMillis();

		for (int i = 0; i < pha.length; i += part)
		{
			list.add(executor.submit(new LineMultiplier(pha, phb, mRows, mColumns, i, i + part)));
		}

		int start = 0;

		for (final Future<double[][]> future : list)
		{
			try
			{
				CF = future.get();

				for (int i = start; i < start + part; i += 1)
				{
					phc[i] = CF[i];
				}
			}
			catch (final Exception ex)
			{
				ex.printStackTrace();
			}

			start += part;
		}

		executor.shutdown();
		PrintMatrix(phc, mRows, System.currentTimeMillis() - beginTime);
	}
}