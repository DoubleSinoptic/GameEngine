#pragma once
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "Core/Core.h"
#include "Vector2.h"
#include "Math.h"

namespace ge
{

	class alignas(16) Vector3
	{
	public:
		union {
			struct {
				scalar x, y, z;
			};
			scalar   floats[3];
		};

		constexpr Vector3() noexcept :
			x(0.0), y(0.0), z(0.0)
		{}

		constexpr Vector3(scalar v) noexcept :
			x(v), y(v), z(v)
		{}

		constexpr Vector3(const Vector2 & a, scalar x2) noexcept :
			x(a.x), y(a.y), z(x2)
		{}

		constexpr Vector3(scalar x0, scalar x1, scalar x2) noexcept :
			x(x0), y(x1), z(x2)
		{}

		constexpr scalar operator[](usize i) const noexcept {
			return floats[i];
		}

		constexpr scalar& operator[](usize i) noexcept {
			return floats[i];
		}

		constexpr Vector3 operator-() const noexcept
		{
			return Vector3(-x, -y, -z);
		}

		String toString() const noexcept;
	};

	constexpr Vector3 Vector3Zero = Vector3();
	constexpr Vector3 Vector3One = Vector3(1.0, 1.0, 1.0);
	constexpr Vector3 Vector3Up = Vector3(0.0, 1.0, 0.0);
	constexpr Vector3 Vector3Right = Vector3(1.0, 0.0, 0.0);
	constexpr Vector3 Vector3Forward = Vector3(0.0, 0.0, 1.0);

	constexpr Vector3 operator/(const Vector3& a, scalar v) noexcept
	{
		return Vector3(a.x / v, a.y / v, a.z / v);
	}

	constexpr Vector3 operator/(scalar v, const Vector3& a) noexcept
	{
		return Vector3(a.x / v, a.y / v, a.z / v);
	}

	constexpr Vector3 operator*(const Vector3& a, scalar v) noexcept
	{
		return Vector3(a.x * v, a.y * v, a.z * v);
	}

	constexpr Vector3 operator*(scalar v, const Vector3& a) noexcept
	{
		return Vector3(a.x * v, a.y * v, a.z * v);
	}

	constexpr Vector3 operator*(const Vector3& a, const Vector3& b) noexcept
	{
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	constexpr Vector3 operator/(const Vector3& a, const Vector3& b) noexcept
	{
		return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
	}

	constexpr Vector3 operator+(const Vector3& a, const Vector3& b) noexcept
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	constexpr Vector3 operator-(const Vector3& a, const Vector3& b) noexcept
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	constexpr scalar dot(const Vector3& a, const Vector3& b) noexcept
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	constexpr scalar length(const Vector3& a) noexcept
	{
		return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	constexpr Vector3 cross(const Vector3& a, const Vector3& b) noexcept
	{
		return Vector3(
			(a.y * b.z) - (b.y * a.z),
			(b.x * a.z) - (a.x * b.z),
			(a.x * b.y) - (b.x * a.y)
		);
	}

	constexpr Vector3 normalize(const Vector3& a) noexcept
	{
		scalar l = length(a);
		if (l != 0) {
			l = 1.0 / l;
			return Vector3(a.x * l, a.y * l, a.z * l);
		}
		return Vector3(0.0);
	}

	constexpr Vector3 sin(const Vector3& a) noexcept
	{
		return Vector3(sin(a.x), sin(a.y), sin(a.z));
	}

	constexpr Vector3 cos(const Vector3& a) noexcept
	{
		return Vector3(cos(a.x), cos(a.y), cos(a.z));
	}

	constexpr void sinCos(const Vector3& a, Vector3& s, Vector3& c) noexcept
	{
		s = sin(a);
		c = cos(a);
	}

}

#endif