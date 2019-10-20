#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Core/StringFormat.h"
namespace ge 
{
	String Vector2::toString() const noexcept 
	{
		return format("({0}, {1})", x, y);
	}

	String Vector3::toString() const noexcept
	{
		return format("({0}, {1}, {2})", x, y, z);
	}

	String Vector4::toString() const noexcept
	{
		return format("({0}, {1}, {2}, {3})", x, y, z, w);
	}

	String Quaternion::toString() const noexcept
	{
		const Vector3 eulers = toEuler();
		return format("({0}, {1}, {2}, {3} - {4})", x, y, z, w, eulers);
	}

	String Matrix4::toString() const noexcept
	{
		return format("({0}, {1}, {2}, {3})", rows[0], rows[1], rows[2], rows[4]);
	}

	String Matrix3::toString() const noexcept
	{
		return format("({0}, {1}, {2})", rows[0], rows[1], rows[2]);
	}
}