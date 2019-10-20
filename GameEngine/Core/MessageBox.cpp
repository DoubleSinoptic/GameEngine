#include "MessageBox.h"

#ifdef _WIN32
#	include <Windows.h>
#   undef MessageBox
#endif

namespace ge
{

	void MessageBox::showInfo(const String& messsage, const String& title, MessageBoxType type)
	{
		#ifdef _WIN32
			UINT windowsType = MB_OK;
			switch (type)
			{
			case ge::MBT_ERROR:
				windowsType |= MB_ICONERROR;
				break;
			case ge::MBT_INFO:
			default:
				windowsType |= MB_ICONINFORMATION;
			}
			MessageBoxW(nullptr, (const wchar_t*)messsage.c_str(), (const wchar_t*)title.c_str(), windowsType);
		#endif
	}

}