#pragma once
#ifndef __ALLIGNTOOLS_H__
#define __ALLIGNTOOLS_H__

#include "./Core.h"

namespace ge
{
	template<typename T>
	class PageController 
	{
		Vector<T> m_values;
	public:
		
	};

	class AllignTools 
	{
	public:
		static constexpr usize allignSize(usize len, usize allign) 
		{
			return ((len + allign - 1) / allign) * allign;
		}

	
	};
}




#endif