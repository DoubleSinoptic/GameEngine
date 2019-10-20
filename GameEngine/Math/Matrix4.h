#pragma once
#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "Core/Core.h"
#include "Vector4.h"

namespace ge
{

	struct Matrix4
	{
	public:
		Vector4 rows[4];

		constexpr Matrix4() noexcept :
			rows{
				Vector4(0.0, 0.0, 0.0, 0.0),
				Vector4(0.0, 0.0, 0.0, 0.0),
				Vector4(0.0, 0.0, 0.0, 0.0),
				Vector4(0.0, 0.0, 0.0, 0.0)
		}
		{}

		constexpr Matrix4(float diag) noexcept :
			rows{
				Vector4(diag, 0.0, 0.0, 0.0),
				Vector4(0.0, diag, 0.0, 0.0),
				Vector4(0.0, 0.0, diag, 0.0),
				Vector4(0.0, 0.0, 0.0, diag)
		}
		{}

		constexpr Matrix4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) noexcept :
			rows{
				Vector4(row0.x, row0.y, row0.z, row0.w),
				Vector4(row1.x, row1.y, row1.z, row1.w),
				Vector4(row2.x, row2.y, row2.z, row2.w),
				Vector4(row3.x, row3.y, row3.z, row3.w)
		}
		{}

		constexpr Matrix4(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3) noexcept :
			rows{
				Vector4(x0, y0, z0, w0),
				Vector4(x1, y1, z1, w1),
				Vector4(x2, y2, z2, w2),
				Vector4(x2, y2, z2, w3),
		}
		{}

		constexpr Vector4& operator[](usize i) noexcept
		{
			return rows[i];
		}

		constexpr const Vector4& operator[](usize i) const noexcept
		{
			return rows[i];
		}

		String toString() const noexcept;
	};

}

#endif