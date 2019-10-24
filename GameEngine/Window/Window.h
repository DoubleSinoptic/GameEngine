#pragma once
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Core/Core.h"

namespace ge
{
	class Window 
	{
	protected:
		Window() {}
	public:
		static Ptr<Window> create(uint32 w, uint32 h, const String& title);
	
		virtual void maximaze() noexcept = 0;
		virtual void restore() noexcept = 0;

		virtual void setLocation(int32 x, int32 y) = 0;
		virtual void setSize(uint32 w, uint32 h) = 0;

		virtual void getSize(uint32& w, uint32 h) const noexcept = 0;
		virtual void getLocation(int32& x, int32 y) const noexcept = 0;

		virtual String getTitle() const = 0;
		virtual void setTitle(const String& title) = 0;
		
		virtual void setFullsreen(bool value) = 0;
		
		virtual void dispatchMessages() = 0;
		virtual bool isClosed() const noexcept = 0;
	};
}

#endif