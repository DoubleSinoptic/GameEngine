#include "Collider.h"
#include "RigidBody.h"
#include "Internal.h"
#include "Physics.h"
#include "Core/Debug.h" 

namespace ge
{
	Collider::Collider() :
		m_shape(nullptr),
		m_static(nullptr)
	{
		setType(getTypeName<Collider>());
	}

	Collider::~Collider()
	{
		deactiveStatic();
		auto shape = getShape();
		if (shape) {
			shape->release();
		}	
	}

	physx::PxShape* Collider::getShape() noexcept
	{
		return m_shape;
	}

	void Collider::setShape(physx::PxShape* shape)
	{
		RigidBody* rg = getGameObject()->getComponent<RigidBody>();
		if (m_shape && rg) {
			rg->removeCollider(this);
		}
		m_shape = shape;
		if (m_shape && rg) {
			rg->addCollider(this);
		}
		if (!rg) {
			activateStatic();
		}
	}

	void Collider::activateStatic()
	{
		if (m_static) {
			Debug::log("Invalid staticbody activation.");
			return;
		}
		activate(CA_TRANSFORM);
		m_static =  Physics::instance().getPhysics()->createRigidStatic(PxTransform(toPx(getGameObject()->position()), toPx(getGameObject()->rotation())));
		m_static->attachShape(*getShape());
		Physics::instance().getMainScene()->addActor(*m_static);
	}

	void Collider::deactiveStatic()
	{
		if (!m_static) {
			Debug::log("Invalid staticbody activation.");
			return;
		}
		deactivete(CA_TRANSFORM);
		Physics::instance().getMainScene()->removeActor(*m_static);
		m_static->detachShape(*getShape());
		m_static->release();
		m_static = nullptr;
	}

	void Collider::setPosition(const Vector3& vec)
	{
		m_shape->setLocalPose(PxTransform(toPx(vec), toPx(QuaternionIdentity)));
	}

	Vector3 Collider::getPosition() const
	{
		return toSge(m_shape->getLocalPose().p);
	}

	void Collider::transformChanged()
	{
		m_static->setGlobalPose(PxTransform(toPx(getGameObject()->position()), toPx(getGameObject()->rotation())));
	}

	void Collider::serialize(BinaryWriter* writer) const
	{
		writer->write<Vector3>(getPosition());
	}

	void Collider::deserialize(BinaryReader* reader)
	{
		setPosition(reader->read<Vector3>());
	}

}