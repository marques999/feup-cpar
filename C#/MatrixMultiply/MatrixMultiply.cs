using System;

class MatrixProduct
{
    static void Main(string[] args)
    {
        int userChoice = 1;
        var instance = new MatrixOperations();

        do
        {
            Console.WriteLine("+---------------------------------------+");
            Console.WriteLine("| 1. Multiplication                     |");
            Console.WriteLine("| 2. Line Multiplication                |");
            Console.WriteLine("| 3. Parallel Multiplication            |");
            Console.WriteLine("| 4. Parallel Line Multiplication       |");
            Console.WriteLine("+---------------------------------------+");
            Console.WriteLine("| 0. Exit                               |");
            Console.WriteLine("+---------------------------------------+");
            Console.WriteLine();
            Console.Write("Selection: ? ");

            int numberLines;
            int numberColumns;
            string userString = Console.ReadLine();

            if (int.TryParse(userString, out userChoice) && (userChoice > 0 && userChoice <= 4))
            {
                Console.Write("Dimensions: lins cols ? ");

                string[] tokens = Console.ReadLine().Split();

                if (tokens.Length == 2 && int.TryParse(tokens[0], out numberLines) && int.TryParse(tokens[1], out numberColumns))
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
            else
            {
                Console.WriteLine("Error: invalid input, please try again...\n");
                userChoice = 1;
            }
        } while (userChoice != 0);
    }
}