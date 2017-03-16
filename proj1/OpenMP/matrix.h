#ifndef __MATRIX_H_
#define __MATRIX_H_

void MultiplyNaive(int mRows, int mColumns);
void MultiplyNaiveParallel(int mRows, int mColumns, int mThreads);
void MultiplyLine(int mRows, int mColumns);
void MultiplyLineParallel(int mRows, int mColumns, int mThreads);

#endif /* __MATRIX_H_ */