package cpar_proj1;

import java.util.concurrent.Callable;

public class DefaultMultiplier extends AbstractMultiplier implements Callable<double[][]>
{
	public DefaultMultiplier(double[][] a, double[][] b, int r, int c, int s, int e)
	{
		super(a, b, r, c, s, e);
	}

	@Override
	public double[][] call()
	{
		for (int i = start; i < end; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				double sumValue = 0;

				for (int k = 0; k < columns; k++)
				{
					sumValue += A[i][k] * B[k][j];
				}

				C[i][j] = sumValue;
			}
		}

		return C;
	}
}