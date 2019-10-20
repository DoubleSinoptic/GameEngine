#pragma once
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Core/Core.h"
#include "Vector3.h"
#include "Math.h"

namespace ge
{

	class alignas(16) Quaternion
	{
	public:
		union {
			struct {
				scalar x, y, z, w;
			};
			scalar   floats[4];
		};

		constexpr Quaternion() noexcept :
			x(0.0), y(0.0), z(0.0), w(1.0)
		{}

		constexpr Quaternion(scalar x0, scalar x1, scalar x2, scalar x3) noexcept :
			x(x0), y(x1), z(x2), w(x3)
		{}


		constexpr Quaternion(const Vector3 & axis, scalar angle) noexcept :
			Quaternion()
		{
			const scalar halfAngle = (angle * (Pi / scalar(180))) / scalar(2);
			const scalar halfSin = sin(halfAngle);
			x = axis.x * halfSin;
			y = axis.y * halfSin;
			z = axis.z * halfSin;
			w = cos(halfAngle);
		}

		constexpr Quaternion(const Vector3 & eulerAngels) noexcept :
			Quaternion(eulerAngels.x, eulerAngels.y, eulerAngels.z)
		{}

		constexpr Quaternion(scalar xa, scalar ya, scalar za) noexcept :
			Quaternion()
		{
			Vector3 s, c;
			sinCos(scalar(0.5) * Vector3(xa, ya, za) * (Pi / scalar(180)), s, c);
			x = s.x * c.y * c.z + s.y * s.z * c.x;
			y = s.y * c.x * c.z - s.x * s.z * c.y;
			z = s.z * c.x * c.y - s.x * s.y * c.z;
			w = c.x * c.y * c.z + s.y * s.z * s.x;
		}

		constexpr Vector3 getXYZ() const noexcept
		{
			return Vector3(x, y, z);
		}

		constexpr Vector3 toEuler() const noexcept
		{
			Vector3 Result;
			toEuler(Result);
			return Result;
		}

		constexpr bool toEuler(Vector3 & angles) const noexcept
		{
			const scalar tx = x + x;
			const scalar ty = y + y;
			const scalar fTz = z + z;
			const scalar tyy = ty * y;
			const scalar tzz = fTz * z;
			const scalar txx = tx * x;
			const scalar tyz = fTz * y;
			const scalar twx = tx * w;
			const scalar twy = ty * w;
			const scalar twz = fTz * w;
			const scalar txy = ty * x;
			const scalar txz = fTz * x;

			const scalar m00 = 1.0f - (tyy + tzz);
			const scalar m11 = 1.0f - (txx + tzz);
			const scalar m22 = 1.0f - (txx + tyy);
			const scalar m12 = tyz - twx;
			const scalar m02 = txz + twy;
			const scalar m10 = txy + twz;
			const scalar m01 = txy - twz;

			angles.x = -asin(m12);
			if (angles.x < HalfPi)
			{
				if (angles.x > -HalfPi)
				{
					angles.y = atan2(m02, m22);
					angles.z = atan2(m10, m11);
					angles = angles * (scalar(180) / Pi);
					return true;
				}
				else
				{
					angles.x = -HalfPi;
					angles.y = atan2(-m01, m00);
					angles.z = scalar(0);
					angles = angles * (scalar(180) / Pi);
					return false;
				}
			}
			else
			{
				angles.x = HalfPi;
				angles.y = atan2(m01, m00);
				angles.z = scalar(0);
				angles = angles * (scalar(180) / Pi);
				return false;
			}
			return false;
		}

		constexpr float operator[](usize i) const noexcept
		{
			return floats[i];
		}

		constexpr float& operator[](usize i) noexcept
		{
			return floats[i];
		}

		String toString() const noexcept;
	};

	constexpr scalar length(const Quaternion& quat)
	{
		return sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
	}

	constexpr scalar squanceLength(const Quaternion& quat)
	{
		return quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
	}

	constexpr Quaternion inverse(const Quaternion& quat)
	{
		const scalar normal = squanceLength(quat);
		if (normal > scalar(0.0)) {
			const scalar inverseNormal = scalar(1.0) / normal;
			return Quaternion(-quat.x * inverseNormal, -quat.y * inverseNormal, -quat.z * inverseNormal, quat.w * inverseNormal);
		}
		return Quaternion(0.0, 0.0, 0.0, 0.0);
	}

	constexpr Quaternion operator*(const Quaternion& q1, const Quaternion& q2) noexcept
	{
		return Quaternion(
			q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
			q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
			q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
			q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
	}

	constexpr Vector3 operator*(const Quaternion& q, const Vector3& v) noexcept
	{
		Vector3 t = 2 * cross(q.getXYZ(), v);
		return v + q.w * t + cross(q.getXYZ(), t);
	}

}
#endif