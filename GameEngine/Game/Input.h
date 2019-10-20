#pragma once
#ifndef __INPUT_H__
#define __INPUT_H__

#include "Core/Core.h"
#include "Math/Vector2.h"

namespace ge
{
	enum class MouseKey
	{
		Left,
		Right,
		Cross
	};

	class GE_EXPORT InputSource
	{
	public:
		virtual ~InputSource() = default;
		virtual Vector2 getMousePoisition() const = 0;
		virtual bool getKeyDown(uint32 key) const = 0;
		virtual bool getMouseKeyDown(MouseKey mouseKey) const = 0;
	};

	class Input 
	{
	public:
		static InputSource& instance();
		static void setCurrentInputSource(Ptr<InputSource> inputSource);

		static Vector2 mousePosition();
		static bool keyDown(uint32 key);
		static bool mouseKeyDown(MouseKey mouseKey);
	};
}

#endif