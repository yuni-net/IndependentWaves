namespace iw
{
	unsigned long seeds4xrandom[4];

	// this is "Xorshift"
	unsigned long xrandom()
	{
		unsigned long & x = seeds4xrandom[0];
		unsigned long & y = seeds4xrandom[1];
		unsigned long & z = seeds4xrandom[2];
		unsigned long & w = seeds4xrandom[3];
		unsigned long t;

		t = x ^ (x << 11);
		x = y; y = z; z = w;
		return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	}

	void xrandom_init(unsigned long seed)
	{
		for (int i = 0; i < 4; ++i)
		{
			seeds4xrandom[i] = seed = 1812433253U * (seed ^ (seed >> 30)) + (i+1);
		}
	}
}