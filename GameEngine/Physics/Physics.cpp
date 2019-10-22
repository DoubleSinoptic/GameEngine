#include "Physics.h"
#include "Internal.h"
#include "RigidBody.h"
#include "Core/Debug.h"
#include "Core/ThreadPool.h"

namespace ge 
{
	Ptr<Physics> currentPhysics;
	void Physics::setCurrentPhysics(Ptr<Physics> physics)
	{
		currentPhysics = physics;
	}

	Physics& Physics::instance()
	{
		return *currentPhysics;
	}

	class PxCpFs : public PxCpuDispatcher
	{
		ThreadPool m_sheduler;
	public:
		PxCpFs() : m_sheduler()
		{}

		virtual void submitTask(PxBaseTask& task) override
		{
			m_sheduler.runTask([&]() {
				task.run();
				task.release();
			});
		}

		virtual uint32_t getWorkerCount() const override
		{
			return m_sheduler.threadCount();
		}
	};

	class PhysicsFileds 
	{
	public:
		PxCpFs				    fpse;
		PxDefaultAllocator		dAlloc;
		PxDefaultErrorCallback  dError;
	};

	Physics::Physics(int) :
		m_isFetchResults(false),
		m_staticFileds(snew<PhysicsFileds>())
	{
		m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_staticFileds->dAlloc, m_staticFileds->dError);
		geAssert(m_foundation);
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale());
		geAssert(m_physics);
		m_dispatcher = &m_staticFileds->fpse;
		geAssert(m_dispatcher);
		PxSceneDesc desk = (PxTolerancesScale());
		desk.gravity = PxVec3(0, -9.82f, 0);
		desk.cpuDispatcher = m_dispatcher;
		desk.filterShader = PxDefaultSimulationFilterShader;
		desk.flags = PxSceneFlag::eENABLE_ACTIVE_ACTORS;
		desk.broadPhaseType = PxBroadPhaseType::eSAP;
		m_scene = m_physics->createScene(desk);
		geAssert(m_scene);
		m_defaultMaterial = m_physics->createMaterial(1.0f, 1.0f, 0.5f);
		geAssert(m_defaultMaterial);
	}

	Physics::~Physics()
	{
		m_defaultMaterial->release();
		m_scene->release();
		m_physics->release();
		m_foundation->release();
	}

	void Physics::update(float dt, bool mt)
	{
		m_scene->simulate(dt, nullptr);
		if (!m_scene->fetchResults(true))
			Debug::log("Error of feth results.");
		m_isFetchResults = true;
		PxU32 numActiveTransforms;
		PxActor** activeTransforms = m_scene->getActiveActors(numActiveTransforms);
		for (usize i = 0; i < numActiveTransforms; i++) {
			const PxRigidBody* ac = static_cast<const PxRigidBody*>(activeTransforms[i]);
			RigidBody* rb = reinterpret_cast<RigidBody*>(ac->userData);
			PxTransform tr = ac->getGlobalPose();
			rb->getGameObject()->setTransfrom(toSge(tr.p), toSge(tr.q));
		}
		m_isFetchResults = false;
	}

	bool Physics::isFetchResults() const
	{
		return m_isFetchResults;
	}

	physx::PxPhysics* Physics::getPhysics() const
	{
		return m_physics;
	}

	physx::PxScene* Physics::getMainScene() const
	{
		return m_scene;
	}

	physx::PxMaterial* Physics::getMaterial() const 
	{
		return m_defaultMaterial;
	}
}

