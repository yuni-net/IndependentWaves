#ifndef iw_filesize_h_
#define iw_filesize_h_

#include "iw_typedef.h"

namespace iw
{
	/*
	 * pathに指定したファイルのサイズ(byte)を取得する
	 */
	uint filesize(const char * path);
}

#endif