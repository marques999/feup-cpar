package cpar_proj2;

import java.util.Scanner;

public class SieveEratosthenes
{
	private static Scanner scanner = new Scanner(System.in);

	public static void main(String[] args)
	{
		int userChoice = 1;

		do
		{
			String[] tokens = null;
			System.out.println("+---------------------------------------+");
			System.out.println("| 1. Sequential byte[]                  |");
			System.out.println("| 2. Sequential boolean[]               |");
			System.out.println("| 3. Sequential ArrayList<Boolean>      |");
			System.out.println("| 4. Sequential Bitset                  |");
			System.out.println("| 5. Sequential Lambda                  |");
			System.out.println("+---------------------------------------+");
			System.out.println("| 6. Benchmark [Sequential Naive]       |");
			System.out.println("| 7. Benchmark [Sequential Line]        |");
			System.out.println("| 8. Benchmark [Parallel Naive]         |");
			System.out.println("| 9. Benchmark [Parallel Line]          |");
			System.out.println("+---------------------------------------+");
			System.out.println("| 0. Exit                               |");
			System.out.println("+---------------------------------------+");
			System.out.println();
			System.out.print("Selection ? ");
			String userString = scanner.nextLine();

			try
			{
				userChoice = Integer.parseInt(userString);

				if (userChoice > 0 && userChoice <= 8)
				{
					if (userChoice <= 4)
					{
						System.out.print("Maximum Value ? ");
						tokens = scanner.nextLine().split(" ");
					}

					try
					{
						if (userChoice > 4 || tokens.length == 1)
						{
							switch (userChoice)
							{
							case 1:
								SieveSequential.ByteArray(Integer.parseInt(tokens[0]));
								break;
							case 2:
								SieveSequential.BoolArray(Integer.parseInt(tokens[0]));
								break;
							case 3:
								SieveSequential.BoolVector(Integer.parseInt(tokens[0]));
								break;
							case 4:
								SieveSequential.BitSet(Integer.parseInt(tokens[0]));
								break;
							case 5:
								SieveSequential.LambdaFunctional(Integer.parseInt(tokens[0]));
								break;
							case 6:
								System.out.println("Running single-threaded line multiplication benchmark...\n");
								//runBenchmark(600, 3000, 400, new MultiplyLine());
								//runBenchmark(4000, 10000, 2000, new MultiplyLine());
								break;
							case 7:
								System.out.println("Running multi-threaded naive multiplication benchmark...\n");
								//runBenchmark(600, 3000, 400, new MultiplyNaiveParallel());
								break;
							case 8:
								System.out.println("Running multi-threaded line multiplication benchmark...\n");
								//runBenchmark(600, 3000, 400, new MultiplyLineParallel());
								//runBenchmark(4000, 10000, 2000, new MultiplyLineParallel());
								break;
							}
						}
					}
					catch (final NumberFormatException ex)
					{
						System.out.println("Error: non-numeric input is unsupported, try again.");
						userChoice = 1;
					}
				}
				else
				{
					System.out.println("Error: non-numeric input is unsupported, try again.");
					userChoice = 1;
				}
			}
			catch (final NumberFormatException ex)
			{
				System.out.println("Error: non-numeric input is unsupported, try again.");
				userChoice = 1;
			}
		} while (userChoice != 0);
	}
}