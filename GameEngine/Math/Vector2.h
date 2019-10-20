#pragma once
#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include "Core/Core.h"

namespace ge
{
	class alignas(16) Vector2
	{
	public:
		union {
			struct {
				scalar x, y;
			};
			scalar   floats[2];
		};

		constexpr Vector2() noexcept :
			x(0.0), y(0.0)
		{}

		constexpr Vector2(scalar v) noexcept :
			x(v), y(v)
		{}

		constexpr Vector2(scalar x0, scalar x1) noexcept :
			x(x0), y(x1)
		{}

		constexpr scalar operator[](usize i) const noexcept
		{
			return floats[i];
		}

		constexpr scalar& operator[](usize i) noexcept
		{
			return floats[i];
		}

		String toString() const noexcept;
	};

	constexpr Vector2 Vector2Zero = Vector2();
	constexpr Vector2 Vector2One = Vector2(1.0, 1.0);

}

#endif