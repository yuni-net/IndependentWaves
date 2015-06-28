#ifndef iw_xy2radian_h_
#define iw_xy2radian_h_

namespace iw
{
	/*
	 * ベクトル(1, 0)と、指定したベクトル(x, y)のなす角(ラジアン)を取得する ************
	 */
	float xy2radian(float x, float y);

	// 指定するxy座標が示すベクトルが単位ベクトルならこちらを使った方が効率的
	float xy2radian_unit(float x, float y);
	// *****************************************************************************

	/*
	 * ラジアンを度に変換する
	 */
	float radian2degree(float radian);
}

#endif