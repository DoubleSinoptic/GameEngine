#pragma once
/*
	written by Alex Kharin in 2019
	https://github.com/DoubleSinoptic
*/
#ifndef	__SGE_PHYSICS_BOXCOLIIDER_H__
#define __SGE_PHYSICS_BOXCOLIIDER_H__

#include "Core/Core.h"
#include "Collider.h"
#include "Math/Vector3.h"

namespace ge
{
	class GE_EXPORT BoxCollider : public Collider
	{
		Vector3 extents;
	public:
		virtual void initialize() override;
		~BoxCollider();
		void setExtents(const Vector3& ext);

		void serialize(BinaryWriter* writer) const override;
		void deserialize(BinaryReader* reader) override;
	};
}
#endif