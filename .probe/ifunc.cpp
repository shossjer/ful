int dispatch(int) __attribute__((ifunc ("resolve")));

int dispatch_identity(int x) { return x; }

extern "C"
{
	static int (* resolve())(int)
	{
		return dispatch_identity;
	}
}

int main()
{
	return dispatch(0);
}
