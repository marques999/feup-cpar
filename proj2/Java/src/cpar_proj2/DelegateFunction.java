package cpar_proj2;

public abstract class DelegateFunction
{
	public abstract void execute(int maximumValue);
}

class SequentialBoolArray extends DelegateFunction
{
	@Override
	public void execute(int maximumValue)
	{
		SieveSequential.BoolArray(maximumValue);
	}
}

class SequentialBoolVector extends DelegateFunction
{
	@Override
	public void execute(int maximumValue)
	{
		SieveSequential.BoolVector(maximumValue);
	}
}

class SequentialByteArray extends DelegateFunction
{
	@Override
	public void execute(int maximumValue)
	{
		SieveSequential.ByteArray(maximumValue);
	}
}

class SequentialNoSmallest extends DelegateFunction
{
	@Override
	public void execute(int maximumValue)
	{
		SieveSequential.BitSet(maximumValue);
	}
}