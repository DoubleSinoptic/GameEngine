#include "SphereCollider.h"
#include "Internal.h"
#include "Physics.h"

namespace ge
{
	void SphereCollider::initialize()
	{
		radius = 0.5;
		PxSphereGeometry geometry(radius);
		setShape(Physics::instance().getPhysics()->createShape(geometry, *(Physics::instance().getMaterial()), true));
	}

	SphereCollider::~SphereCollider()
	{
	}

	void SphereCollider::setRadius(scalar value)
	{
		radius = value;
		getShape()->setGeometry(PxSphereGeometry(radius));
	}

	void SphereCollider::serialize(BinaryWriter* writer) const
	{
		Collider::serialize(writer);
		writer->write<float>(radius);
	}

	void SphereCollider::deserialize(BinaryReader* reader)
	{
		Collider::deserialize(reader);
		setRadius(reader->read<float>());
	}
}