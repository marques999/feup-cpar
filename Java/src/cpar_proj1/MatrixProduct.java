package cpar_proj1;

import java.util.Scanner;

public class MatrixProduct
{
	private static Matrix instance = new Matrix();
	private static Scanner scanner = new Scanner(System.in);

	public static void main(String[] args)
	{
		int userChoice = 1;

		do
		{
			System.out.println("+---------------------------------------+");
			System.out.println("| 1. Multiplication                     |");
			System.out.println("| 2. Line Multiplication                |");
			System.out.println("| 3. Parallel Multiplication            |");
			System.out.println("| 4. Parallel Line Multiplication       |");
			System.out.println("+---------------------------------------+");
			System.out.println("| 0. Exit                               |");
			System.out.println("+---------------------------------------+");
			System.out.println();
			System.out.print("Selection: ? ");

			final String userString = scanner.nextLine();

			try
			{
				userChoice = Integer.parseInt(userString);

				if (userChoice > 0 && userChoice < 5)
				{
					System.out.print("Dimensions: lins cols ? ");

					final String[] tokens = scanner.nextLine().split(" ");

					try
					{
						int numberLines = Integer.parseInt(tokens[0]);
						int numberColumns = Integer.parseInt(tokens[1]);

						if (tokens.length == 2)
						{
							switch (userChoice)
							{
							case 1:
								instance.Multiply(numberLines, numberColumns);
								break;
							case 2:
								instance.MultiplyLine(numberLines, numberColumns);
								break;
							case 3:
								instance.MultiplyParallel(numberLines, numberColumns);
								break;
							case 4:
								instance.MultiplyLineParallel(numberLines, numberColumns);
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