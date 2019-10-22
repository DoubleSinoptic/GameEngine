#pragma once
#ifndef	__GEPHYSICSCOLIIDER_H__
#define __GEPHYSICSCOLIIDER_H__

#include "Core/Core.h"
#include "Game/GameObject.h"
#include "InternalShared.h"

namespace ge
{

	class GE_EXPORT Collider : public Component
	{
		physx::PxRigidStatic*   m_static;
		physx::PxShape*			m_shape;
	public:
		Collider();
		~Collider();
		physx::PxShape* getShape() noexcept;
		void			setShape(physx::PxShape* shape);
		void			activateStatic();
		void			deactiveStatic();
		void			setPosition(const Vector3& vec);
		Vector3			getPosition() const;
		void			transformChanged() override;

		void			serialize(BinaryWriter* writer) const override;
		void			deserialize(BinaryReader* reader) override;
	};
}

#endif