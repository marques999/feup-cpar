using System;
using System.Diagnostics;
using System.Threading.Tasks;

class Matrix
{
    private double[][] InitializeMatrixA(int sz)
    {
        double[][] m = new double[sz][];

        for (int i = 0; i < sz; i++)
        {
            m[i] = new double[sz];

            for (int j = 0; j < sz; j++)
            {
                m[i][j] = 1.0;
            }
        }

        return m;
    }

    private double[][] InitializeMatrixB(int sz)
    {
        double[][] m = new double[sz][];

        for (int i = 0; i < sz; i++)
        {
            m[i] = new double[sz];

            for (int j = 0; j < sz; j++)
            {
                m[i][j] = i + 1;
            }
        }

        return m;
    }

    private double[][] InitializeMatrixC(int sz)
    {
        double[][] m = new double[sz][];

        for (int i = 0; i < sz; i++)
        {
            m[i] = new double[sz];
        }

        return m;
    }

    private void PrintMatrix(double[][] matrix, int sz, Stopwatch sw)
    {
        Console.WriteLine("Time: {0:F3} seconds", sw.ElapsedMilliseconds / 1000.0);

        for (int j = 0; j < Math.Min(10, sz); j++)
        {
            Console.Write(matrix[0][j] + " ");
        }

        Console.WriteLine("\n");
    }

    public void Multiply(int aRows, int bRows)
    {
        double[][] pha = InitializeMatrixA(aRows);
        double[][] phb = InitializeMatrixB(bRows);
        double[][] phc = InitializeMatrixC(aRows);

        var stopWatch = Stopwatch.StartNew();

        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < bRows; j++)
            {
                double sumValue = 0;

                for (int k = 0; k < aRows; k++)
                {
                    sumValue += pha[i][k] * phb[k][j];
                }

                phc[i][j] = sumValue;
            }
        }

        stopWatch.Stop();
        PrintMatrix(phc, bRows, stopWatch);
    }

    public void MultiplyLine(int aRows, int bRows)
    {
        double[][] pha = InitializeMatrixA(aRows);
        double[][] phb = InitializeMatrixB(bRows);
        double[][] phc = InitializeMatrixC(aRows);

        var stopWatch = Stopwatch.StartNew();

        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < bRows; j++)
            {
                for (int k = 0; k < aRows; k++)
                {
                    phc[i][k] += pha[i][j] * phb[j][k];
                }
            }
        }

        stopWatch.Stop();
        PrintMatrix(phc, bRows, stopWatch);
    }

    public void MultiplyParallel(int aRows, int bRows)
    {
        double[][] pha = InitializeMatrixA(aRows);
        double[][] phb = InitializeMatrixB(bRows);
        double[][] phc = InitializeMatrixC(aRows);

        var stopWatch = Stopwatch.StartNew();

        Parallel.For(0, aRows, i =>
        {
            for (int j = 0; j < bRows; j++)
            {
                double sumValue = 0;

                for (int k = 0; k < aRows; k++)
                {
                    sumValue += pha[i][k] * phb[k][j];
                }

                phc[i][j] = sumValue;
            }
        });

        stopWatch.Stop();
        PrintMatrix(phc, bRows, stopWatch);
    }

    public void MultiplyLineParallel(int aRows, int bRows)
    {
        double[][] pha = InitializeMatrixA(aRows);
        double[][] phb = InitializeMatrixB(bRows);
        double[][] phc = InitializeMatrixC(aRows);

        var stopWatch = Stopwatch.StartNew();

        Parallel.For(0, aRows, i =>
        {
            for (int j = 0; j < bRows; j++)
            {
                for (int k = 0; k < aRows; k++)
                {
                    phc[i][k] += pha[i][j] * phb[j][k];
                }
            }
        });

        stopWatch.Stop();
        PrintMatrix(phc, bRows, stopWatch);
    }
}