package cpar_proj1;

public abstract class DelegateFunction
{
	public abstract void execute(int mRows, int mColumns);
}

class MultiplyNaive extends DelegateFunction
{
	@Override
	public void execute(int mRows, int mColumns)
	{
		MatrixOperations.Multiply(mRows, mColumns);
	}
}

class MultiplyLine extends DelegateFunction
{
	@Override
	public void execute(int mRows, int mColumns)
	{
		MatrixOperations.MultiplyLine(mRows, mColumns);
	}
}

class MultiplyNaiveParallel extends DelegateFunction
{
	@Override
	public void execute(int mRows, int mColumns)
	{
		MatrixOperations.MultiplyParallel(mRows, mColumns);
	}
}

class MultiplyLineParallel extends DelegateFunction
{
	@Override
	public void execute(int mRows, int mColumns)
	{
		MatrixOperations.MultiplyLineParallel(mRows, mColumns);
	}
}