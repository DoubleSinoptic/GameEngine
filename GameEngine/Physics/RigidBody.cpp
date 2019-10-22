#include "RigidBody.h"
#include "Internal.h"
#include "Physics.h"

namespace ge
{

	void RigidBody::initialize()
	{
		activate(CA_TRANSFORM);
		m_rigiBody = Physics::instance().getPhysics()->createRigidDynamic(PxTransform(toPx(getGameObject()->position()), toPx(getGameObject()->rotation())));
		m_rigiBody->userData = this;
		auto c = getGameObject()->getComponents<Collider>();
		for (auto& x : c) {
			x->deactiveStatic();
			m_rigiBody->attachShape(*x->getShape());
		}
		Physics::instance().getMainScene()->addActor(*m_rigiBody);
	}

	RigidBody::~RigidBody()
	{
		auto c = getGameObject()->getComponents<Collider>();
		for (auto& x : c) {
			m_rigiBody->detachShape(*x->getShape());
			x->activateStatic();
		}
		Physics::instance().getMainScene()->removeActor(*m_rigiBody);
		m_rigiBody->release();
	}

	void RigidBody::transformChanged()
	{
		if (Physics::instance().isFetchResults())
			return;
		auto p = getGameObject()->position();
		auto q = getGameObject()->rotation();
		m_rigiBody->setGlobalPose(PxTransform(toPx(p), toPx(q)));
	}

	Vector3 RigidBody::getLinearVeloicty() const noexcept 
	{
		return toSge(m_rigiBody->getLinearVelocity());
	}

	Vector3 RigidBody::getAnigularVeloicty() const noexcept 
	{
		return toSge(m_rigiBody->getAngularVelocity());
	}

	void RigidBody::setLinearVeloicty(const Vector3& v) noexcept 
	{
		m_rigiBody->setLinearVelocity(toPx(v));
	}

	void RigidBody::setAnigularVeloicty(const Vector3& v) noexcept 
	{
		m_rigiBody->setAngularVelocity(toPx(v));
	}

	float RigidBody::getMass() const noexcept 
	{
		return m_rigiBody->getMass();
	}

	void RigidBody::setMass(float mass) noexcept 
	{
		m_rigiBody->setMass(mass);
	}

	void RigidBody::addCollider(Collider* collider)
	{
		m_rigiBody->attachShape(*collider->getShape());
	}

	void RigidBody::removeCollider(Collider* collider) 
	{
		m_rigiBody->detachShape(*collider->getShape());
	}

	void RigidBody::serialize(BinaryWriter* writer) const
	{
		writer->write<Vector3>(getLinearVeloicty());
		writer->write<Vector3>(getAnigularVeloicty());
		writer->write<float>(getMass());
	}

	void RigidBody::deserialize(BinaryReader* reader)
	{
		setLinearVeloicty(reader->read<Vector3>());
		setAnigularVeloicty(reader->read<Vector3>());
		setMass(reader->read<float>());
	}

}