#ifndef iw_xrandom_h_
#define iw_xrandom_h_

namespace iw
{
	// �������擾����
	// �������K�{�I���L��xrandom_init�֐����g�p���邱�ƁB
	// �擾�����l���S�ă[���ɂȂ�ꍇ�͏���������Ă��Ȃ��؋�
	// �����񏉊������Ă���薳��
	unsigned long xrandom();

	// xrandom�֐��Ŏ擾���闐���̃V�[�h��ݒ肷��
	// 0�ȊO�ł���Ή��ł��悢
	// ���̑����̗���������Ɠ����悤�ɁA�����V�[�h��ݒ肷��Ώ�ɓ������������������
	void xrandom_init(unsigned long seed);

}

#endif