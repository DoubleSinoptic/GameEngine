#pragma once
#ifndef	__GEPHYSICSRIGIDBODY_H__
#define __GEPHYSICSRIGIDBODY_H__

#include "Core/Core.h"
#include "Game/GameObject.h"
#include "Collider.h"
#include "Physics.h"
#include "InternalShared.h"

namespace ge
{

	class GE_EXPORT RigidBody : public Component
	{
		physx::PxRigidBody* m_rigiBody;
	public:
		virtual void initialize() override;
		~RigidBody();
		virtual void transformChanged() override;

		Vector3 getLinearVeloicty() const noexcept;
		Vector3 getAnigularVeloicty() const noexcept;

		void setLinearVeloicty(const Vector3&) noexcept;
		void setAnigularVeloicty(const Vector3&) noexcept;

		float getMass() const noexcept;
		void  setMass(float mass) noexcept;

		void addCollider(Collider* collider);
		void removeCollider(Collider* collider);

		void serialize(BinaryWriter* writer) const override;
		void deserialize(BinaryReader* reader) override;
	};

}
#endif