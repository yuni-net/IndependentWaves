#ifndef iw_cnct_h_
#define iw_cnct_h_

#include <sstream>

namespace iw
{
	/*
	 * 文字列、数値、その他諸々を文字列に変換して連結するためのクラス
	 * ex:
	 * const char * str = iw::cnct() << "result is " << (23 + 235) < " !";	// "result is 258 !"
	 * <<で文字列、数値等を連結する。最後だけ<を使うとconst char *が返されるからこれを利用する。
	 * ちなみに<<=を使うことでstd::string &が返される。この値をコピーするか、const std::string &型変数で参照して使うこと。
	 * ちなみにキャスト演算子をオーバーロードしているため、<や<<=を使わなくてもconst char *型かstd::string型で受け取るとき暗黙的にキャストされる。
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