#include "iw_radian.h"
#include "constant.h"

namespace iw
{
	float radian2degree(float radian)
	{
		return radian*360.0f / (2.0f*iw::pi);
	}
}