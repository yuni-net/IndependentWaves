#ifndef iw_cnct_h_
#define iw_cnct_h_

#include <sstream>

namespace iw
{
	/*
	 * ������A���l�A���̑����X�𕶎���ɕϊ����ĘA�����邽�߂̃N���X
	 * ex:
	 * const char * str = iw::cnct() << "result is " << (23 + 235) < " !";	// "result is 258 !"
	 * <<�ŕ�����A���l����A������B�Ōゾ��<���g����const char *���Ԃ���邩�炱��𗘗p����B
	 * ���Ȃ݂�<<=���g�����Ƃ�std::string &���Ԃ����B���̒l���R�s�[���邩�Aconst std::string &�^�ϐ��ŎQ�Ƃ��Ďg�����ƁB
	 * ���Ȃ݂ɃL���X�g���Z�q���I�[�o�[���[�h���Ă��邽�߁A<��<<=���g��Ȃ��Ă�const char *�^��std::string�^�Ŏ󂯎��Ƃ��ÖٓI�ɃL���X�g�����B
	 */
	class cnct
	{
	public:
 
		template <typename X>
		cnct & operator << (const X &x){
			myoss << x;
			return *this;
		}

		template <typename X>
		std::string & operator <<= (const X &x){
			myoss << x;
			return mystr = myoss.str();
		}

		template <typename X>
		const char * operator < (const X &x){
			myoss << x;
			mystr = myoss.str();
			return mystr.c_str();
		}

		operator const std::string (){
			mystr = myoss.str();
			return mystr;
		}

		operator const char * () const{
			mystr = myoss.str();
			return mystr.c_str();
		}

	private:
		std::ostringstream myoss;
		mutable std::string mystr;

	};

}

#endif