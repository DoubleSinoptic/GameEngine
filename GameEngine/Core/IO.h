#pragma once
#ifndef __IO_H__
#define __IO_H__

#include "./Core.h"

namespace ge
{
	class FileUtils 
	{
	public:
		static Vector<byte> readAllContents(const String& path);
		static void writeAllContents(const Vector<byte>& data, const String& path);
	};
}

#endif