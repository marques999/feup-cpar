package cpar_proj1;

public class AbstractMultiplier
{
	protected double[][] A;
	protected double[][] B;
	protected double[][] C;

	protected AbstractMultiplier(double[][] a, double[][] b, int r, int c, int s, int e)
	{
		A = a;
		B = b;
		end = e;
		start = s;
		rows = r;
		columns = c;
		C = new double[a.length][b[0].length];
	}

	protected int rows;
	protected int columns;
	protected int end;
	protected int start;
}