package cpar_proj1;

public class AbstractMultiplier
{
	protected double[][] A;
	protected double[][] B;
	protected double[][] C;

	protected AbstractMultiplier(double[][] a, double[][] b, int s, int e)
	{
		A = a;
		B = b;
		end = e;
		start = s;
		C = new double[a.length][b[0].length];
	}

	protected int end;
	protected int start;
}