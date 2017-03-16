using System;
using System.Diagnostics;
using System.Threading.Tasks;

class MatrixOperations
{
    private double[][] InitializeMatrixA(int mRows, int mColumns)
    {
        double[][] m = new double[mRows][];

        for (int i = 0; i < mRows; i++)
        {
            m[i] = new double[mColumns];

            for (int j = 0; j < mColumns; j++)
            {
                m[i][j] = 1.0;
            }
        }

        return m;
    }

    private double[][] InitializeMatrixB(int mRows, int mColumns)
    {
        double[][] m = new double[mRows][];

        for (int i = 0; i < mRows; i++)
        {
            m[i] = new double[mColumns];

            for (int j = 0; j < mColumns; j++)
            {
                m[i][j] = i + 1;
            }
        }

        return m;
    }

    private double[][] InitializeMatrixC(int mRows, int mColumns)
    {
        double[][] m = new double[mRows][];

        for (int i = 0; i < mRows; i++)
        {
            m[i] = new double[mColumns];
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

    public void Multiply(int mRows, int mColumns)
    {
        double[][] pha = InitializeMatrixA(mRows, mColumns);
        double[][] phb = InitializeMatrixB(mColumns, mRows);
        double[][] phc = InitializeMatrixC(mRows, mRows);

        var stopWatch = Stopwatch.StartNew();

        for (int i = 0; i < mRows; i++)
        {
            for (int j = 0; j < mRows; j++)
            {
                double sumValue = 0;

                for (int k = 0; k < mColumns; k++)
                {
                    sumValue += pha[i][k] * phb[k][j];
                }

                phc[i][j] = sumValue;
            }
        }

        stopWatch.Stop();
        PrintMatrix(phc, mRows, stopWatch);
    }

    public void MultiplyLine(int mRows, int mColumns)
    {
        double[][] pha = InitializeMatrixA(mRows, mColumns);
        double[][] phb = InitializeMatrixB(mColumns, mRows);
        double[][] phc = InitializeMatrixC(mRows, mRows);

        var stopWatch = Stopwatch.StartNew();

        for (int i = 0; i < mRows; i++)
        {
            for (int j = 0; j < mColumns; j++)
            {
                for (int k = 0; k < mRows; k++)
                {
                    phc[i][k] += pha[i][j] * phb[j][k];
                }
            }
        }

        stopWatch.Stop();
        PrintMatrix(phc, mRows, stopWatch);
    }

    public void MultiplyParallel(int mRows, int mColumns)
    {
        double[][] pha = InitializeMatrixA(mRows, mColumns);
        double[][] phb = InitializeMatrixB(mColumns, mRows);
        double[][] phc = InitializeMatrixC(mRows, mRows);

        var stopWatch = Stopwatch.StartNew();

        Parallel.For(0, mRows, i =>
        {
            for (int j = 0; j < mRows; j++)
            {
                double sumValue = 0;

                for (int k = 0; k < mColumns; k++)
                {
                    sumValue += pha[i][k] * phb[k][j];
                }

                phc[i][j] = sumValue;
            }
        });

        stopWatch.Stop();
        PrintMatrix(phc, mRows, stopWatch);
    }

    public void MultiplyLineParallel(int mRows, int mColumns)
    {
        double[][] pha = InitializeMatrixA(mRows, mColumns);
        double[][] phb = InitializeMatrixB(mColumns, mRows);
        double[][] phc = InitializeMatrixC(mRows, mRows);

        var stopWatch = Stopwatch.StartNew();

        Parallel.For(0, mRows, i =>
        {
            for (int j = 0; j < mColumns; j++)
            {
                for (int k = 0; k < mRows; k++)
                {
                    phc[i][k] += pha[i][j] * phb[j][k];
                }
            }
        });

        stopWatch.Stop();
        PrintMatrix(phc, mRows, stopWatch);
    }
}