#pragma once

#ifndef	__GEPHYSICSSPHERECOLIIDER_H__
#define __GEPHYSICSSPHERECOLIIDER_H__

#include "Core/Core.h"
#include "Collider.h"

namespace ge
{

	class GE_EXPORT SphereCollider : public Collider
	{
		scalar radius;
	public:
		virtual void initialize() override;
		~SphereCollider();
		void setRadius(scalar value);

		void serialize(BinaryWriter* writer) const override;
		void deserialize(BinaryReader* reader) override;
	};

}
#endif