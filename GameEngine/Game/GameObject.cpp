#include "GameObject.h"
#include "GameObjectManager.h"

namespace ge 
{
	GameObject::GameObject() :
		GameObject(u"gameobject")
	{}

	GameObject::GameObject(const String& s) :
		m_parent(nullptr),
		m_name(s),
		m_placeId(InvalidPlace)
	{
		m_placeId = GameObjectManager::instance().gameObjects().size();
		GameObjectManager::instance().gameObjects().push_back(this);
	}

	GameObject::~GameObject()
	{
		if (m_parent) 
		{
			removeFind(m_parent->m_childrens, this);
			m_parent->onConstructionChanged();
			m_parent = nullptr;
		}

		for (auto x : m_components) 
		{
			delete x;
		}

		while (!m_childrens.empty())
		{
			delete m_childrens.back();
		}
		if (removeSwap(GameObjectManager::instance().gameObjects(), m_placeId))
			GameObjectManager::instance().gameObjects()[m_placeId]->m_placeId = m_placeId;
		m_placeId = InvalidPlace;
	}

	void GameObject::setParent(GameObject* parent)
	{
		if (m_parent) 
		{
			removeFind(m_parent->m_childrens, this);
			m_parent->onConstructionChanged();
			m_parent = nullptr;
		}
		m_parent = parent;
		if (m_parent) 
		{
			m_parent->m_childrens.push_back(this);
			m_parent->onConstructionChanged();
		}
		else 
		{
			onConstructionChanged();
		}
	}

	void GameObject::serialize(BinaryWriter* writer) const
	{
		writer->writeString(u"__gameobject");
		writer->writeString(m_name);
		writer->write<Vector3>(m_localPosition);
		writer->write<Quaternion>(m_localRotation);
		writer->write<uint64>(m_components.size());
		for (auto& x : m_components) 
		{
			writer->write<uint32>(writer->instanceId(x));
			x->serialize(writer);
		}
		writer->write<uint64>(m_childrens.size());
		for (auto& x : m_childrens) 
		{
			x->serialize(writer);
		}
	}

	void GameObject::deserialize(BinaryReader* reader)
	{
		geAssert(reader->readString() == u"__gameobject");
		m_name = reader->readString();
		setLocalPosition(reader->read<Vector3>());
		setLocalRotation(reader->read<Quaternion>());
		uint64 nComponents = reader->read<uint64>();
		for (uint64 i = 0; i < nComponents; i++)
		{
			ISerializable* sr = reader->instanceNew(reader->read<uint32>());
			Component* cmp = static_cast<Component*>(sr);
			cmp->setGameObject(this);
			cmp->initialize();
			m_components.push_back(cmp);
			cmp->deserialize(reader);
		}
		uint64 nChild = reader->read<uint64>();
		for (uint64 i = 0; i < nChild; i++) 
		{
			GameObject* sr = new GameObject();
			sr->setParent(this);
			sr->deserialize(reader);
		}
	}

	Vector3 GameObject::localPosition() const noexcept
	{
		return m_localPosition;
	}

	Quaternion GameObject::localRotation() const noexcept
	{
		return m_localRotation;
	}

	Vector3 GameObject::position() const noexcept
	{
		return m_globlPosition;
	}

	Quaternion GameObject::rotation() const noexcept
	{
		return m_globlRotation;
	}

	Vector3 GameObject::up() const noexcept
	{
		return m_globlRotation * Vector3Up;
	}

	Vector3 GameObject::forward() const noexcept
	{
		return m_globlRotation * Vector3Forward;
	}

	Vector3 GameObject::right() const noexcept
	{
		return m_globlRotation * Vector3Right;
	}

	void GameObject::setRotation(const Quaternion& rot) noexcept
	{
		if (m_parent)
		{
			Quaternion q = inverse(m_parent->rotation());
			m_localRotation = (q * rot);
		}
		else 
		{
			m_localRotation = rot;
		}
		onTransformChanged();
	}

	void GameObject::setPosition(const Vector3& pos) noexcept
	{
		if (m_parent)
		{
			m_localPosition = inverse(m_parent->rotation()) * (pos - m_parent->position());
		}
		else 
		{
			m_localPosition = pos;
		}
		onTransformChanged();
	}

	void GameObject::setTransfrom(const Vector3& pos, const Quaternion& rot) noexcept
	{
		if (m_parent) 
		{
			Vector3 parPos = m_parent->position();
			Quaternion invParRot = inverse(m_parent->rotation());
			m_localPosition = invParRot * (pos - parPos);
			m_localRotation = (invParRot * rot);
		}
		else 
		{
			m_localPosition = pos;
			m_localRotation = rot;
		}
		onTransformChanged();
	}

	void GameObject::setLocalRotation(const Quaternion& rot) noexcept
	{
		m_localRotation = rot;
		onTransformChanged();
	}

	void GameObject::setLocalPosition(const Vector3& position) noexcept
	{
		m_localPosition = position;
		onTransformChanged();
	}

	void GameObject::onTransformChanged()
	{
		if (m_parent) 
		{
			m_globlPosition = m_parent->rotation() * m_localPosition + m_parent->position();
			m_globlRotation = m_parent->rotation() * m_localRotation;
		}
		else 
		{
			m_globlPosition = m_localPosition;
			m_globlRotation = m_localRotation;
		}

		for (auto x : m_childrens)
			x->onTransformChanged();
		for (auto x : m_transformChangedComponents)
			x->transformChanged();
	}

	void GameObject::onConstructionChanged()
	{
		for (auto x : m_childrens)
			x->onConstructionChanged();
		for (auto x : m_transformChangedComponents)
			x->componentManipulation();
	}

	void GameObject::clearScene()
	{
		while (!GameObjectManager::instance().gameObjects().empty())
			delete GameObjectManager::instance().gameObjects().back();
	}

	Vector<GameObject*> GameObject::find(const String& expr)
	{
		Vector<GameObject*> gameObjects;
		for (auto object : GameObjectManager::instance().gameObjects()) 
		{
			if (object->m_name == expr)
			{
				gameObjects.push_back(object);
			}
		}
		return gameObjects;
	}
}

