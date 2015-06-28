#ifndef iw_cast_h_
#define iw_cast_h_

namespace iw
{
	template<typename type, typename Y>
	type cast(Y& req)
	{
		return static_cast<type>(req);
	}

	template<typename pointer_type>
	pointer_type pointer_cast(const void* req)
	{
		return static_cast<pointer_type>(req);
	}

	template<typename pointer_type>
	pointer_type pointer_cast(void* req)
	{
		return static_cast<pointer_type>(req);
	}

	template<typename type>
	type tovalue_cast(const void* req)
	{
		return reinterpret_cast<type>(req);
	}

	template<typename pointer_type, typename Y>
	pointer_type topointer_cast(Y& req)
	{
		return reinterpret_cast<pointer_type>(req);
	}

	template<typename pointer_type, typename Y>
	pointer_type up_cast(Y base)
	{
		return dynamic_cast<pointer_type>(base);
	}

	template<typename type>
	type & unconst_cast(const type & req)
	{
		return *(const_cast<type*>(&req));
	}	// can't use pointer
}

#endif
