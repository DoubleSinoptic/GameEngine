#include "Input.h"

namespace ge
{
	Ptr<InputSource> currentInputSource;

	InputSource& Input::instance()
	{
		return *currentInputSource;
	}

	void Input::setCurrentInputSource(Ptr<InputSource> inputSource)
	{
		currentInputSource = inputSource;
	}

	Vector2 Input::mousePosition()
	{
		return Vector2();
	}
	bool Input::keyDown(uint32 key)
	{
		return false;
	}
	bool Input::mouseKeyDown(MouseKey mouseKey)
	{
		return false;
	}

}

