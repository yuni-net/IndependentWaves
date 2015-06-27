#include "filesize.h"
#include <cstdio>
#include "cast.h"

#pragma warning(push)
#pragma warning(disable:4996)

namespace iw
{
	uint filesize(const char * path)
	{
		fpos_t fsize = 0;

		FILE * fp = fopen(path, "rb");

		if (fp == nullptr)
		{
			return 0;
		}

		/* ファイルサイズを調査 */
		fseek(fp, 0, SEEK_END);
		fgetpos(fp, &fsize);

		fclose(fp);

		return iw::cast<uint>(fsize);
	}
}

#pragma warning(pop)