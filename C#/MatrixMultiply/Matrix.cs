using System;
using System.Diagnostics;
using System.Threading.Tasks;

class Matrix
{
    private double[,] InitializeMatrixA(int sz)
    {
        double[,] m = new double[sz, sz];

        for (int i = 0; i < sz; i++)
        {
            for (int j = 0; j < sz; j++)
            {
                m[i, j] = 1.0;
            }
        }

        return m;
    }

    private double[,] InitializeMatrixB(int sz)
    {
        double[,] m = new double[sz, sz];

        for (int i = 0; i < sz; i++)
        {
            for (int j = 0; j < sz; j++)
            {
                m[i, j] = i + 1;
            }
        }

        return m;
    }

    private void PrintMatrix(double[,] matrix, int sz)
    {
        Console.WriteLine("Result matrix: ");

        for (int j = 0; j < Math.Min(10, sz); j++)
        {
            Console.Write(matrix[0, j] + " ");
        }

        Console.WriteLine();
    }

    public void Multiply(int aRows, int bRows)
    {
        double[,] pha = InitializeMatrixA(aRows);
        double[,] phb = InitializeMatrixB(bRows);
        double[,] phc = new double[aRows, aRows];

        var stopWatch = Stopwatch.StartNew();

        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < bRows; j++)
            {
                double sumValue = 0;

                for (int k = 0; k < aRows; k++)
                {
                    sumValue += pha[i, k] * phb[k, j];
                }

                phc[i, j] = sumValue;
            }
        }

        stopWatch.Stop();
        Console.WriteLine("Time: {0:F3} seconds", stopWatch.ElapsedMilliseconds / 1000.0);
        PrintMatrix(phc, bRows);
    }

    public void MultiplyLine(int aRows, int bRows)
    {
        double[,] pha = InitializeMatrixA(aRows);
        double[,] phb = InitializeMatrixB(bRows);
        double[,] phc = new double[aRows, aRows];

        var stopWatch = Stopwatch.StartNew();

        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < bRows; j++)
            {
                for (int k = 0; k < aRows; k++)
                {
                    phc[i, k] += pha[i, j] * phb[j, k];
                }
            }
        }

        stopWatch.Stop();
        Console.WriteLine("Time: {0:F3} seconds", stopWatch.ElapsedMilliseconds / 1000.0);
        PrintMatrix(phc, bRows);
    }

    public void MultiplyParallel(int aRows, int bRows)
    {
        double[,] pha = InitializeMatrixA(aRows);
        double[,] phb = InitializeMatrixB(bRows);
        double[,] phc = new double[aRows, aRows];

        var stopWatch = Stopwatch.StartNew();

        Parallel.For(0, aRows, i =>
        {
            for (int j = 0; j < bRows; j++)
            {
                double sumValue = 0;

                for (int k = 0; k < aRows; k++)
                {
                    sumValue += pha[i, k] * phb[k, j];
                }

                phc[i, j] = sumValue;
            }
        });

        stopWatch.Stop();
        Console.WriteLine("Time: {0:F3} seconds", stopWatch.ElapsedMilliseconds / 1000.0);
        PrintMatrix(phc, bRows);
    }

    public void MultiplyLineParallel(int aRows, int bRows)
    {
        double[,] pha = InitializeMatrixA(aRows);
        double[,] phb = InitializeMatrixB(bRows);
        double[,] phc = new double[aRows, aRows];

        var stopWatch = Stopwatch.StartNew();

        Parallel.For(0, aRows, i =>
        {
            for (int j = 0; j < bRows; j++)
            {
                for (int k = 0; k < aRows; k++)
                {
                    phc[i, k] += pha[i, j] * phb[j, k];
                }
            }
        });

        stopWatch.Stop();
        Console.WriteLine("Time: {0:F3} seconds", stopWatch.ElapsedMilliseconds / 1000.0);
        PrintMatrix(phc, bRows);
    }
}