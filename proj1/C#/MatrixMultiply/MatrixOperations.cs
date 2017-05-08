using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;

namespace MatrixMultiply
{
    /// <summary>
    /// 
    /// </summary>
    internal class MatrixOperations
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="mRows"></param>
        /// <param name="mColumns"></param>
        /// <returns></returns>
        private static double[][] InitializeMatrixA(int mRows, int mColumns)
        {
            var m = new double[mRows][];

            for (var i = 0; i < mRows; i++)
            {
                m[i] = new double[mColumns];

                for (var j = 0; j < mColumns; j++)
                {
                    m[i][j] = 1.0;
                }
            }

            return m;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mRows"></param>
        /// <param name="mColumns"></param>
        /// <returns></returns>
        private static double[][] InitializeMatrixB(int mRows, int mColumns)
        {
            var m = new double[mRows][];

            for (var i = 0; i < mRows; i++)
            {
                m[i] = new double[mColumns];

                for (var j = 0; j < mColumns; j++)
                {
                    m[i][j] = i + 1;
                }
            }

            return m;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mRows"></param>
        /// <param name="mColumns"></param>
        /// <returns></returns>
        private static double[][] InitializeMatrixC(int mRows, int mColumns)
        {
            var m = new double[mRows][];

            for (var i = 0; i < mRows; i++)
            {
                m[i] = new double[mColumns];
            }

            return m;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="matrix"></param>
        /// <param name="sz"></param>
        /// <param name="sw"></param>
        private static void PrintMatrix(IReadOnlyList<double[]> matrix, int sz, Stopwatch sw)
        {
            Console.WriteLine($"Time: {sw.ElapsedMilliseconds / 1000.0:F3} seconds");

            for (var j = 0; j < Math.Min(10, sz); j++)
            {
                Console.Write(matrix[0][j] + " ");
            }

            Console.WriteLine("\n");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mRows"></param>
        /// <param name="mColumns"></param>
        public static void Multiply(int mRows, int mColumns)
        {
            var pha = InitializeMatrixA(mRows, mColumns);
            var phb = InitializeMatrixB(mColumns, mRows);
            var phc = InitializeMatrixC(mRows, mRows);
            var stopWatch = Stopwatch.StartNew();

            for (var i = 0; i < mRows; i++)
            {
                for (var j = 0; j < mRows; j++)
                {
                    double sumValue = 0;

                    for (var k = 0; k < mColumns; k++)
                    {
                        sumValue += pha[i][k] * phb[k][j];
                    }

                    phc[i][j] = sumValue;
                }
            }

            stopWatch.Stop();
            PrintMatrix(phc, mRows, stopWatch);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mRows"></param>
        /// <param name="mColumns"></param>
        public static void MultiplyLine(int mRows, int mColumns)
        {
            var pha = InitializeMatrixA(mRows, mColumns);
            var phb = InitializeMatrixB(mColumns, mRows);
            var phc = InitializeMatrixC(mRows, mRows);
            var stopWatch = Stopwatch.StartNew();

            for (var i = 0; i < mRows; i++)
            {
                for (var j = 0; j < mColumns; j++)
                {
                    for (var k = 0; k < mRows; k++)
                    {
                        phc[i][k] += pha[i][j] * phb[j][k];
                    }
                }
            }

            stopWatch.Stop();
            PrintMatrix(phc, mRows, stopWatch);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mRows"></param>
        /// <param name="mColumns"></param>
        public static void MultiplyParallel(int mRows, int mColumns)
        {
            var pha = InitializeMatrixA(mRows, mColumns);
            var phb = InitializeMatrixB(mColumns, mRows);
            var phc = InitializeMatrixC(mRows, mRows);
            var stopWatch = Stopwatch.StartNew();

            Parallel.For(0, mRows, i =>
            {
                for (var j = 0; j < mRows; j++)
                {
                    double sumValue = 0;

                    for (var k = 0; k < mColumns; k++)
                    {
                        sumValue += pha[i][k] * phb[k][j];
                    }

                    phc[i][j] = sumValue;
                }
            });

            stopWatch.Stop();
            PrintMatrix(phc, mRows, stopWatch);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mRows"></param>
        /// <param name="mColumns"></param>
        public static void MultiplyLineParallel(int mRows, int mColumns)
        {
            var pha = InitializeMatrixA(mRows, mColumns);
            var phb = InitializeMatrixB(mColumns, mRows);
            var phc = InitializeMatrixC(mRows, mRows);
            var stopWatch = Stopwatch.StartNew();

            Parallel.For(0, mRows, i =>
            {
                for (var j = 0; j < mColumns; j++)
                {
                    for (var k = 0; k < mRows; k++)
                    {
                        phc[i][k] += pha[i][j] * phb[j][k];
                    }
                }
            });

            stopWatch.Stop();
            PrintMatrix(phc, mRows, stopWatch);
        }
    }
}