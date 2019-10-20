#pragma once
#ifndef __MATRIX3_H__
#define __MATRIX3_H__

#include "Core/Core.h"
#include "Vector3.h"

namespace ge
{
	struct Matrix3
	{
	public:
		Vector3 rows[3];

		constexpr Matrix3() noexcept :
			rows{
				Vector3(0.0, 0.0, 0.0),
				Vector3(0.0, 0.0, 0.0),
				Vector3(0.0, 0.0, 0.0)
		}
		{}

		constexpr Matrix3(float diag) noexcept :
			rows{
				Vector3(diag, 0.0, 0.0),
				Vector3(0.0, diag, 0.0),
				Vector3(0.0, 0.0, diag)
		}
		{}

		constexpr Matrix3(const Vector3& row0, const Vector3& row1, const Vector3& row2) noexcept :
			rows{
				Vector3(row0.x, row0.y, row0.z),
				Vector3(row1.x, row1.y, row1.z),
				Vector3(row2.x, row2.y, row2.z)
		}
		{}

		constexpr Matrix3(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) noexcept :
			rows{
				Vector3(x0, y0, z0),
				Vector3(x1, y1, z1),
				Vector3(x2, y2, z2)
		}
		{}

		constexpr Vector3& operator[](usize i) noexcept
		{
			return rows[i];
		}

		constexpr const Vector3& operator[](usize i) const noexcept
		{
			return rows[i];
		}

		String toString() const noexcept;
	};

}
#endif