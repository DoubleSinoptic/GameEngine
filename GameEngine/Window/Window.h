#pragma once
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Core/Core.h"
#include "Core/Any.h"
#include "WindowManager.h"

namespace ge
{
	struct VideoMod
	{
		int32 refreshRate = -1;
		uint32 widith = 1;
		uint32 height = 1;
	};

	class Monitor
	{
	protected:
		Monitor() {}
	public:
		virtual ~Monitor() = default;
		virtual String getName() const = 0;
		virtual void getPhysicsSize(uint32& w, uint32& h) const = 0;
		virtual void getWorkArea(int32& x, int32& y, uint32& w, uint32& h) const = 0;
		virtual void getLocaiton(int32& w, int32& h) const = 0;
		virtual void getScale(float& w, float& h) const = 0;
		virtual VideoMod getCurrentVideoMod() const = 0;
		virtual Vector<VideoMod> getVideoModes() const = 0;
	};

	class Window 
	{
	protected:
		Window() {}
	public:
		virtual ~Window() = default;

		virtual Any getNativeHandle() const noexcept = 0;

		virtual void maximaze() noexcept = 0;
		virtual void restore() noexcept = 0;

		virtual void setLocation(int32 x, int32 y) = 0;
		virtual void setSize(uint32 w, uint32 h) = 0;

		virtual void getSize(uint32& w, uint32 h) const noexcept = 0;
		virtual void getLocation(int32& x, int32 y) const noexcept = 0;

		virtual String getTitle() const = 0;
		virtual void setTitle(const String& title) = 0;
		
		virtual void setFullscreen(const Ptr<Monitor>& monitor, int32 x, int32 y, const VideoMod& mod) = 0;
		virtual void setWindowed(int32 x, int32 y, uint32 w, uint32 h) = 0;

		virtual bool isClosed() const noexcept = 0;
	};
}

#endif