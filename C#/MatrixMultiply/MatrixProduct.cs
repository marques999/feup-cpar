using System;

class MatrixProduct
{
    static void Main(string[] args)
    {
        int op = 1;
        var instance = new Matrix();

        do
        {
            Console.WriteLine();
            Console.WriteLine("-------------------------");
            Console.WriteLine("1. Multiplication");
            Console.WriteLine("2. Line Multiplication");
            Console.WriteLine("3. Parallel Multiplication");
            Console.WriteLine("4. Parallel Line Multiplication");
            Console.WriteLine();
            Console.WriteLine("0. Exit");
            Console.WriteLine("-------------------------");
            Console.Write("Selection: ? ");

            string input = Console.ReadLine();

            if (int.TryParse(input, out op))
            {
                int numberLines;
                int numberColumns;

                if (op == 1 || op == 2 || op == 3 || op == 4)
                {
                    Console.Write("Dimensions: lins cols ? ");

                    string[] tokens = Console.ReadLine().Split();

                    if (tokens.Length == 2 && int.TryParse(tokens[0], out numberLines) && int.TryParse(tokens[1], out numberColumns))
                    {
                        switch (op)
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
            }
            else
            {
                Console.WriteLine("Error: non-numeric input is unsupported, try again.");
                op = 1;
            }
        } while (op != 0);

        Console.WriteLine();
    }
}