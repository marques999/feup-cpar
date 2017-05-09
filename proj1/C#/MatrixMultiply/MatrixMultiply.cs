using System;

namespace MatrixMultiply
{
    /// <summary>
    /// 
    /// </summary>
    internal class MatrixProduct
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="start"></param>
        /// <param name="end"></param>
        /// <param name="step"></param>
        /// <param name="matrixOperation"></param>
        private static void RunBenchmark(int start, int end, int step, MatrixOperation matrixOperation)
        {
            for (var i = start; i <= end; i += step)
            {
                Console.WriteLine("[" + i + "x" + i + "]");
                matrixOperation(i, i);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mRows"></param>
        /// <param name="mColumns"></param>
        private delegate void MatrixOperation(int mRows, int mColumns);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        private static void Main(string[] args)
        {
            var userChoice = 1;

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

                var numberLines = 0;
                var numberColumns = 0;
                var userString = Console.ReadLine();
                string[] tokens = null;

                if (int.TryParse(userString, out userChoice) && userChoice > 0 && userChoice <= 8)
                {
                    if (userChoice <= 4)
                    {
                        Console.Write("Dimensions: lins cols ? ");

                        var readLine = Console.ReadLine();

                        if (readLine != null)
                        {
                            tokens = readLine.Split();
                        }
                    }

                    if (tokens == null || userChoice <= 4 && (tokens.Length != 2 || !int.TryParse(tokens[0], out numberLines) || !int.TryParse(tokens[1], out numberColumns)))
                    {
                        continue;
                    }

                    switch (userChoice)
                    {
                    case 1:
                        MatrixOperations.Multiply(numberLines, numberColumns);
                        break;
                    case 2:
                        MatrixOperations.MultiplyLine(numberLines, numberColumns);
                        break;
                    case 3:
                        MatrixOperations.MultiplyParallel(numberLines, numberColumns);
                        break;
                    case 4:
                        MatrixOperations.MultiplyLineParallel(numberLines, numberColumns);
                        break;
                    case 5:
                        Console.WriteLine("Running single-threaded naive multiplication benchmark...\n");
                        RunBenchmark(600, 3000, 400, MatrixOperations.Multiply);
                        break;
                    case 6:
                        Console.WriteLine("Running single-threaded line multiplication benchmark...\n");
                        RunBenchmark(600, 3000, 400, MatrixOperations.MultiplyLine);
                        RunBenchmark(4000, 10000, 2000, MatrixOperations.MultiplyLine);
                        break;
                    case 7:
                        Console.WriteLine("Running multi-threaded naive multiplication benchmark...\n");
                        RunBenchmark(600, 3000, 400, MatrixOperations.MultiplyParallel);
                        break;
                    case 8:
                        Console.WriteLine("Running multi-threaded line multiplication benchmark...\n");
                        RunBenchmark(600, 3000, 400, MatrixOperations.MultiplyLineParallel);
                        RunBenchmark(4000, 10000, 2000, MatrixOperations.MultiplyLineParallel);
                        break;
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
}