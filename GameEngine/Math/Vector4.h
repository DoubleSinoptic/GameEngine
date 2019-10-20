#pragma once
#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include "Core/Core.h"
#include "Vector3.h"

namespace ge
{
	class alignas(16) Vector4
	{
	public:
		union {
			struct {
				scalar x, y, z, w;
			};
			scalar   floats[4];
		};

		constexpr Vector4() noexcept :
			x(0.0), y(0.0), z(0.0), w(0.0)
		{}

		constexpr Vector4(scalar v) noexcept :
			x(v), y(v), z(v), w(v)
		{}

		constexpr Vector4(const Vector3 & a, scalar x2) noexcept :
			x(a.x), y(a.y), z(a.z), w(x2)
		{}

		constexpr Vector4(scalar x0, scalar x1, scalar x2, scalar x3) noexcept :
			x(x0), y(x1), z(x2), w(x3)
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

	constexpr Vector4 Vector4Zero = Vector4();
	constexpr Vector4 Vector4One = Vector4(1.0, 1.0, 1.0, 1.0);
}

#endif