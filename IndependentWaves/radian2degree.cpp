#include "iw_radian.h"
#include "iw_constant.h"

namespace iw
{
	float radian2degree(float radian)
	{
		return radian*360.0f / (2.0f*iw::pi);
	}
}