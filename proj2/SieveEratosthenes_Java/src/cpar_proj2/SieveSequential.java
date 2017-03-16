package cpar_proj2;

import java.util.BitSet;
import java.util.IntSummaryStatistics;
import java.util.stream.IntStream;
import java.util.ArrayList;

public class SieveSequential
{
	public static void BoolVector(int maximumValue)
	{
		final ArrayList<Boolean> v = new ArrayList<>();

		for (int i = 0; i < maximumValue - 1; i++)
		{
			v.add(true);
		}

		long start = System.currentTimeMillis();

		for (int i = 2; i * i <= maximumValue; i++)
		{
			if (v.get(i - 2))
			{
				for (int j = i * i; j <= maximumValue - 1; j += i)
				{
					v.set(j - 2, false);
				}
			}
		}

		System.out.format("Elapsed: %.3f seconds\n", (System.currentTimeMillis() - start) / 1000.0);
		SieveCommon.printPrimes(v);
	}

	public static void BoolArray(int maximumValue)
	{
		boolean[] v = new boolean[maximumValue - 1];

		for (int i = 0; i < maximumValue - 1; i++)
		{
			v[i] = true;
		}

		long start = System.currentTimeMillis();

		for (int i = 2; i * i <= maximumValue; i++)
		{
			if (v[i - 2])
			{
				for (int j = i * i; j <= maximumValue - 1; j += i)
				{
					v[i - 2] = false;
				}
			}
		}

		System.out.format("Elapsed: %.3f seconds\n", (System.currentTimeMillis() - start) / 1000.0);
		SieveCommon.printPrimes(v);
	}

	public static void ByteArray(int maximumValue)
	{
		int current = 2;
		int smallest = 3;
		byte[] v = new byte[maximumValue - 1];

		for (int i = 0; i < maximumValue - 1; i++)
		{
			v[i] = 1;
		}

		long start = System.currentTimeMillis();

		while (current * current <= maximumValue)
		{
			for (int i = current * current; i <= maximumValue; i += current)
			{
				v[i - 2] = 0;
			}

			for (int i = current + 1; i <= maximumValue; i++)
			{
				if (v[i - 2] == 1)
				{
					smallest = i;
					break;
				}
			}

			current = smallest;
		}

		System.out.format("Elapsed: %.3f seconds\n", (System.currentTimeMillis() - start) / 1000.0);
		SieveCommon.printPrimes(v);
	}

	public static void LambdaFunctional(int maximumValue)
	{
		final BitSet sieve = new BitSet(maximumValue + 1);
		long start = System.currentTimeMillis();

		final IntSummaryStatistics stats = IntStream.rangeClosed(2, maximumValue).filter(x -> !sieve.get(x)).peek(x ->
		{
			if (x * x < maximumValue)
			{
				for (int i = x; i <= maximumValue; i += x)
				{
					sieve.set(i);
				}
			}
		}).summaryStatistics();

		System.out.format("   Elapsed > %.3f seconds\n", (System.currentTimeMillis() - start) / 1000.0);
		System.out.printf("    Primes > %d\n", stats.getCount());
		System.out.printf("      Last > %d%n\n", stats.getMax());
	}

	public static void BitSet(int maximumValue)
	{
		int i = 2;
		int size = maximumValue + 1;
		final BitSet bitSet = new BitSet(size);

		bitSet.flip(2, size);

		long start = System.currentTimeMillis();

		do
		{
			for (int j = i * 2; j < size; j += i)
			{
				bitSet.clear(j);
			}

			i = bitSet.nextSetBit(i + 1);
		} while (i > 0);

		System.out.printf("   Elapsed > %.3f seconds\n", (System.currentTimeMillis() - start) / 1000.0);
		System.out.printf("    Primes > %d\n", bitSet.cardinality());
		System.out.printf("      Last > %d%n\n", bitSet.previousSetBit(bitSet.length()));
	}
}