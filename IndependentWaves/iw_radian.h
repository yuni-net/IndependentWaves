#ifndef iw_xy2radian_h_
#define iw_xy2radian_h_

namespace iw
{
	/*
	 * �x�N�g��(1, 0)�ƁA�w�肵���x�N�g��(x, y)�̂Ȃ��p(���W�A��)���擾���� ************
	 */
	float xy2radian(float x, float y);

	// �w�肷��xy���W�������x�N�g�����P�ʃx�N�g���Ȃ炱������g�������������I
	float xy2radian_unit(float x, float y);
	// *****************************************************************************

	/*
	 * ���W�A����x�ɕϊ�����
	 */
	float radian2degree(float radian);
}

#endif