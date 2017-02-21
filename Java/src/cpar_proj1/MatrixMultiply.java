package cpar_proj1;

import java.util.Scanner;

public class MatrixMultiply
{
	private static Scanner scanner = new Scanner(System.in);

	private static void runBenchmark(int start, int end, int step, final DelegateFunction matrixOperation)
	{
		for (int i = start; i <= end; i += step)
		{
			System.out.println("[" + i + "x" + i + "]");
			matrixOperation.execute(i, i);
		}
	}

	public static void main(String[] args)
	{
		int userChoice = 1;

		do
		{
			String[] tokens = null;
			System.out.println("+---------------------------------------+");
			System.out.println("| 1. Multiplication                     |");
			System.out.println("| 2. Line Multiplication                |");
			System.out.println("| 3. Parallel Multiplication            |");
			System.out.println("| 4. Parallel Line Multiplication       |");
			System.out.println("+---------------------------------------+");
			System.out.println("| 5. Benchmark [Sequential Naive]       |");
			System.out.println("| 6. Benchmark [Sequential Line]        |");
			System.out.println("| 7. Benchmark [Parallel Naive]         |");
			System.out.println("| 8. Benchmark [Parallel Line]          |");
			System.out.println("+---------------------------------------+");
			System.out.println("| 0. Exit                               |");
			System.out.println("+---------------------------------------+");
			System.out.println();
			System.out.print("Selection: ? ");
			String userString = scanner.nextLine();

			try
			{
				userChoice = Integer.parseInt(userString);

				if (userChoice > 0 && userChoice <= 8)
				{
					if (userChoice <= 4)
					{
						System.out.print("Dimensions: lins cols ? ");
						tokens = scanner.nextLine().split(" ");
					}

					try
					{
						if (userChoice > 4 || tokens.length == 2)
						{
							switch (userChoice)
							{
							case 1:
								MatrixOperations.Multiply(Integer.parseInt(tokens[0]), Integer.parseInt(tokens[1]));
								break;
							case 2:
								MatrixOperations.MultiplyLine(Integer.parseInt(tokens[0]), Integer.parseInt(tokens[1]));
								break;
							case 3:
								MatrixOperations.MultiplyParallel(Integer.parseInt(tokens[0]), Integer.parseInt(tokens[1]));
								break;
							case 4:
								MatrixOperations.MultiplyLineParallel(Integer.parseInt(tokens[0]), Integer.parseInt(tokens[1]));
								break;
							case 5:
								System.out.println("Running single-threaded naive multiplication benchmark...\n");
								runBenchmark(600, 3000, 400, new MultiplyNaive());
								break;
							case 6:
								System.out.println("Running single-threaded line multiplication benchmark...\n");
								runBenchmark(600, 3000, 400, new MultiplyLine());
								runBenchmark(4000, 10000, 2000, new MultiplyLine());
								break;
							case 7:
								System.out.println("Running multi-threaded naive multiplication benchmark...\n");
								runBenchmark(600, 3000, 400, new MultiplyNaiveParallel());
								break;
							case 8:
								System.out.println("Running multi-threaded line multiplication benchmark...\n");
								runBenchmark(600, 3000, 400, new MultiplyLineParallel());
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