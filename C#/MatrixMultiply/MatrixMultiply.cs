using System;

class MatrixProduct
{
    private static MatrixOperations instance = new MatrixOperations();
    private delegate void MatrixOperation(int mRows, int mColumns);

    private static void runBenchmark(int start, int end, int step, MatrixOperation matrixOperation)
    {
        for (int i = start; i <= end; i += step)
        {
            Console.WriteLine("[" + i + "x" + i + "]");
            matrixOperation(i, i);
        }
    }

    static void Main(string[] args)
    {
        int userChoice = 1;

        do
        {
            Console.WriteLine("+---------------------------------------+");
            Console.WriteLine("| 1. Multiplication                     |");
            Console.WriteLine("| 2. Line Multiplication                |");
            Console.WriteLine("| 3. Parallel Multiplication            |");
            Console.WriteLine("| 4. Parallel Line Multiplication       |");
            Console.WriteLine("+---------------------------------------+");
            Console.WriteLine("| 5. Benchmark [Sequential Naive]       |");
            Console.WriteLine("| 6. Benchmark [Sequential Line]        |");
            Console.WriteLine("| 7. Benchmark [Parallel Naive]         |");
            Console.WriteLine("| 8. Benchmark [Parallel Line]          |");
            Console.WriteLine("+---------------------------------------+");
            Console.WriteLine("| 0. Exit                               |");
            Console.WriteLine("+---------------------------------------+");
            Console.WriteLine();
            Console.Write("Selection: ? ");

            int numberLines = 0;
            int numberColumns = 0;
            string userString = Console.ReadLine();
            string[] tokens = null;

            if (int.TryParse(userString, out userChoice) && (userChoice > 0 && userChoice <= 8))
            {
                if (userChoice <= 4)
                {
                    Console.Write("Dimensions: lins cols ? ");
                    tokens = Console.ReadLine().Split();
                }

                if (userChoice > 4 || (tokens.Length == 2 && int.TryParse(tokens[0], out numberLines) && int.TryParse(tokens[1], out numberColumns)))
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
                    case 5:
                        Console.WriteLine("Running single-threaded naive multiplication benchmark...\n");
                        runBenchmark(600, 3000, 400, instance.Multiply);
                        break;
                    case 6:
                        Console.WriteLine("Running single-threaded line multiplication benchmark...\n");
                        runBenchmark(600, 3000, 400, instance.MultiplyLine);
                        runBenchmark(4000, 10000, 2000, instance.MultiplyLine);
                        break;
                    case 7:
                        Console.WriteLine("Running multi-threaded naive multiplication benchmark...\n");
                        runBenchmark(600, 3000, 400, instance.MultiplyParallel);
                        break;
                    case 8:
                        Console.WriteLine("Running multi-threaded line multiplication benchmark...\n");
                        runBenchmark(600, 3000, 400, instance.MultiplyLineParallel);
                        break;
                    }
                }
            }
            else
            {
                Console.WriteLine("Error: invalid input, please try again...\n");
                userChoice = 1;
            }
        } while (userChoice != 0);
    }
}