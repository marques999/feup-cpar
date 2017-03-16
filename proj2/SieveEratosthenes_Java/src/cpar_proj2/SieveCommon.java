package cpar_proj2;

import java.util.ArrayList;

public class SieveCommon
{
	private static final int MAXIMUM_PRIMES = 16;

	public static void prettyPrint(int idx, int v)
	{
		if (idx == 1)
		{
			System.out.format("%d\n", v);
		}
		else
		{

			System.out.format("%d, ", v);
		}
	}

	public static void printPrimes(final ArrayList<Boolean> v)
	{
		int rv = 0;
		int i = v.size() - 1;

		for (int j = 0; j < v.size(); j++)
		{
			if (v.get(j))
			{
				rv++;
			}
		}

		System.out.format("#Primes: %d (", rv);
		rv = Math.min(rv, MAXIMUM_PRIMES);

		while (rv > 0)
		{
			if (v.get(i))
			{
				prettyPrint(rv--, i + 2);
			}

			i--;
		}
	}

	public static void printPrimes(boolean[] v)
	{
		int rv = 0;
		int i = v.length - 1;

		for (int j = 0; j < v.length; j++)
		{
			if (v[j])
			{
				rv++;
			}
		}

		System.out.format("#Primes: %d (", rv);
		rv = Math.min(rv, MAXIMUM_PRIMES);

		while (rv > 0)
		{
			if (v[i])
			{
				prettyPrint(rv--, i + 2);
			}

			i--;
		}
	}

	public static void printPrimes(byte[] v)
	{
		int rv = 0;
		int i = v.length - 1;

		for (int j = 0; j < v.length; j++)
		{
			if (v[j] == 1)
			{
				rv++;
			}
		}

		System.out.format("#Primes: %d (", rv);
		rv = Math.min(rv, MAXIMUM_PRIMES);

		while (rv > 0)
		{
			if (v[i] == 1)
			{
				prettyPrint(rv--, i + 2);
			}

			i--;
		}
	}
}