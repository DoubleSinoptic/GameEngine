#pragma once
#ifndef	__GEPHYSICSINTERNAL_H__
#define __GEPHYSICSINTERNAL_H__

#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Core/Core.h"
#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxDefaultSimulationFilterShader.h"

using namespace physx;

inline PxVec3 toPx(const ge::Vector3& vec)
{
	return { vec.x, vec.y, vec.z };
}

inline PxQuat toPx(const ge::Quaternion& vec)
{
	return { vec.x, vec.y, vec.z, vec.w };
}

inline ge::Vector3 toSge(const PxVec3& vec)
{
	return { vec.x, vec.y, vec.z };
}

inline ge::Quaternion toSge(const PxQuat& vec)
{
	return { vec.x, vec.y, vec.z, vec.w };
}

#endif