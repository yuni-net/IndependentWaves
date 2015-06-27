#ifndef iw_xy2radian_h_
#define iw_xy2radian_h_

namespace iw
{

	float xy2radian(float x, float y);

	// 指定するxy座標が示すベクトルが単位ベクトルならこちらを使った方が効率的
	float xy2radian_unit(float x, float y);


	float radian2degree(float radian);
}

#endif