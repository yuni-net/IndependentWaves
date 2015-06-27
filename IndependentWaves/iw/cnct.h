#ifndef iw_cnct_h_
#define iw_cnct_h_

#include <sstream>

namespace iw
{

class cnct
{
	std::ostringstream myoss;
	mutable std::string mystr;
 
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
};

}

#endif