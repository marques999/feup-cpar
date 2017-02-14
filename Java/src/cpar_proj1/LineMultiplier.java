package cpar_proj1;

import java.util.concurrent.Callable;

public class LineMultiplier extends AbstractMultiplier implements Callable<double[][]>
{
	public LineMultiplier(double[][] a, double[][] b, int s, int e)
	{
		super(a, b, s, e);
	}

	@Override
	public double[][] call()
	{
		for (int i = start; i < end; i++)
		{
			for (int k = 0; k < B.length; k++)
			{
				for (int j = 0; j < B[0].length; j++)
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}

		return C;
	}
}