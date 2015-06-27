#ifndef iw_trade_h_
#define iw_trade_h_


namespace iw
{

	template<typename X>
	void trade(X & one, X & another)
	{
		X other = one;
		one = another;
		another = other;
	}

}

#endif