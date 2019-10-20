#pragma once
#ifndef __MATH_H__
#define __MATH_H__

#include "Core/Core.h"
#include "Gcem/gcem.hpp"

namespace ge
{

	constexpr scalar Pi = scalar(3.14159265359);
	constexpr scalar HalfPi = scalar(3.14159265359) / 2;
	constexpr scalar Epsilon = scalar(1.401298E-45);

	constexpr scalar EngineEpsilon = static_cast<scalar>(0.0001);
	constexpr scalar ScalarOne = static_cast<scalar>(1);
	constexpr scalar ScalarZero = static_cast<scalar>(0);

	constexpr scalar toRad(scalar grad) noexcept
	{
		return Pi / static_cast<scalar>(180) * grad;
	}

	constexpr scalar sin(scalar v) noexcept
	{
		return gcem::sin(v);
	}

	constexpr scalar floor(scalar v) noexcept
	{
		return gcem::floor(v);
	}

	constexpr scalar engineMod(scalar v) noexcept
	{
		return floor(v / EngineEpsilon + scalar(0.5)) * EngineEpsilon;
	}

	constexpr scalar abs(scalar v) noexcept
	{
		return gcem::abs(v);
	}

	constexpr scalar cos(scalar v) noexcept
	{
		return gcem::cos(v);
	}

	constexpr scalar asin(scalar v) noexcept
	{
		return gcem::asin(v);
	}

	constexpr scalar acos(scalar v) noexcept
	{
		return gcem::acos(v);
	}

	constexpr scalar tan(scalar v) noexcept
	{
		return gcem::tan(v);
	}

	constexpr scalar atan2(scalar y, scalar x) noexcept
	{
		return gcem::atan2(y, x);
	}

	constexpr scalar atan(scalar v) noexcept
	{
		return gcem::atan(v);
	}

	constexpr scalar sqrt(scalar v) noexcept
	{
		return gcem::sqrt(v);
	}

}

#endif

