package cpar_proj1;

import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Matrix
{
	private double[][] initializeMatrixA(int sz)
	{
		double[][] m = new double[sz][sz];

		for (int i = 0; i < sz; i++)
		{
			for (int j = 0; j < sz; j++)
			{
				m[i][j] = 1.0;
			}
		}

		return m;
	}

	private double[][] initializeMatrixB(int sz)
	{
		double[][] m = new double[sz][sz];

		for (int i = 0; i < sz; i++)
		{
			for (int j = 0; j < sz; j++)
			{
				m[i][j] = i + 1;
			}
		}

		return m;
	}

	private void PrintMatrix(double[][] matrix, int sz, long elapsedTime)
	{
		System.out.format("Time: %3.3f seconds\n", elapsedTime / 1000.0);

		for (int j = 0; j < Math.min(10, sz); j++)
		{
			System.out.print(matrix[0][j] + " ");
		}

		System.out.println("\n");
	}

	public void Multiply(int aRows, int bRows)
	{
		double[][] pha = initializeMatrixA(aRows);
		double[][] phb = initializeMatrixB(bRows);
		double[][] phc = new double[aRows][aRows];

		long start = System.currentTimeMillis();

		for (int i = 0; i < aRows; i++)
		{
			for (int j = 0; j < bRows; j++)
			{
				double sumValue = 0;

				for (int k = 0; k < aRows; k++)
				{
					sumValue += pha[i][k] * phb[k][j];
				}

				phc[i][j] = sumValue;
			}
		}

		PrintMatrix(phc, bRows, System.currentTimeMillis() - start);
	}

	public void MultiplyLine(int aRows, int bRows)
	{
		double[][] pha = initializeMatrixB(aRows);
		double[][] phb = initializeMatrixB(bRows);
		double[][] phc = new double[aRows][aRows];

		long start = System.currentTimeMillis();

		for (int i = 0; i < aRows; i++)
		{
			for (int j = 0; j < bRows; j++)
			{
				for (int k = 0; k < aRows; k++)
				{
					phc[i][k] += pha[i][j] * phb[j][k];
				}
			}
		}

		PrintMatrix(phc, bRows, System.currentTimeMillis() - start);
	}

	private int poolSize = Runtime.getRuntime().availableProcessors();

	public void MultiplyParallel(int aRows, int bRows)
	{
		double[][] pha = initializeMatrixB(aRows);
		double[][] phb = initializeMatrixB(bRows);
		double[][] phc = new double[pha.length][pha[0].length];
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
			list.add(executor.submit(new DefaultMultiplier(pha, phb, i, i + part)));
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
		PrintMatrix(phc, bRows, System.currentTimeMillis() - beginTime);
	}

	public void MultiplyLineParallel(int aRows, int bRows)
	{
		double[][] pha = initializeMatrixB(aRows);
		double[][] phb = initializeMatrixB(bRows);
		double[][] phc = new double[pha.length][pha[0].length];
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
			list.add(executor.submit(new LineMultiplier(pha, phb, i, i + part)));
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
		PrintMatrix(phc, bRows, System.currentTimeMillis() - beginTime);
	}
}