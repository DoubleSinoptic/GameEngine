#pragma once
#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "../Core/Core.h"
#include "InternalShared.h"

namespace ge
{
	class PhysicsFileds;
	class Physics 
	{
		Ptr<PhysicsFileds>		m_staticFileds;
		bool					m_isFetchResults;
		physx::PxFoundation*	m_foundation;
		physx::PxPhysics*		m_physics;
		physx::PxCpuDispatcher* m_dispatcher;
		physx::PxScene*			m_scene;
		physx::PxMaterial*		m_defaultMaterial;
	public:
		static void setCurrentPhysics(Ptr<Physics> physics);
		static Physics& instance();
	
		Physics(int smCount);
		~Physics();

		void update(float dt, bool mt);
		bool isFetchResults() const;

		physx::PxPhysics* getPhysics() const;
		physx::PxScene* getMainScene() const;
		physx::PxMaterial* getMaterial() const;
	};
}

#endif