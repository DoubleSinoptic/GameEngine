#include "BoxCollider.h"
#include "Internal.h"
#include "Physics.h"

namespace ge
{
	void BoxCollider::initialize()
	{	
		extents = Vector3(1.0);
		PxBoxGeometry geometry(toPx(extents * 0.5f));
		setShape(Physics::instance().getPhysics()->createShape(geometry, *(Physics::instance().getMaterial()), true));
	}

	BoxCollider::~BoxCollider()
	{}

	void BoxCollider::setExtents(const Vector3& ext)
	{
		extents = ext;
		getShape()->setGeometry(PxBoxGeometry(toPx(extents * 0.5f)));
	}

	void BoxCollider::serialize(BinaryWriter* writer) const
	{
		Collider::serialize(writer);
		writer->write<Vector3>(extents);
	}

	void BoxCollider::deserialize(BinaryReader* reader)
	{
		Collider::deserialize(reader);
		setExtents(reader->read<Vector3>());
	}
}


