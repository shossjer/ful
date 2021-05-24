__attribute__((ifunc ("resolve"))) int dispatch(int);

int dispatch_identity(int x) { return x; }

extern "C"
{
	int (* resolve())(int)
	{
		return dispatch_identity;
	}
}

int main()
{
	return dispatch(0);
}
