#pragma once
#ifndef __MESSAGEBOX_H__
#define __MESSAGEBOX_H__

#include "Core.h"

namespace ge
{
	enum MessageBoxType {
		MBT_ERROR,
		MBT_INFO
	};

	class GE_EXPORT MessageBox
	{
	public:
		static void showInfo(const String& messsage, const String& title, MessageBoxType type);
	};

}
#endif