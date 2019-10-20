#include "Component.h"
#include "GameObjectManager.h"
#include "GameObject.h"

namespace ge
{
	void Component::activate(ComponentActivationFlags flags)
	{
		auto& gameInstance = GameObjectManager::instance();
		if ((flags & CA_UPDATE) && m_placeId == InvalidPlace)
		{
			m_placeId = gameInstance.components().size();
			gameInstance.components().push_back(this);
		}
		if ((flags & CA_TRANSFORM) && m_transformPlaceId == InvalidPlace)
		{
			m_transformPlaceId = m_gameObject->m_transformChangedComponents.size();
			m_gameObject->m_transformChangedComponents.push_back(this);
		}
		if ((flags & CA_MANIP) && m_manipPlaceId == InvalidPlace)
		{
			m_manipPlaceId = m_gameObject->m_constructionChagnedComponents.size();
			m_gameObject->m_constructionChagnedComponents.push_back(this);
		}
	}

	void Component::deactivete(ComponentActivationFlags flags)
	{
		auto& gameInstance = GameObjectManager::instance();
		if ((flags & CA_UPDATE) && m_placeId != InvalidPlace)
		{
			if (removeSwap(gameInstance.components(), m_placeId))
				gameInstance.components()[m_placeId]->m_placeId = m_placeId;
			m_placeId = InvalidPlace;
		}
		if ((flags & CA_TRANSFORM) && m_transformPlaceId != InvalidPlace)
		{
			if (removeSwap(m_gameObject->m_transformChangedComponents, m_transformPlaceId))
				m_gameObject->m_transformChangedComponents[m_transformPlaceId]->m_transformPlaceId = m_transformPlaceId;
			m_transformPlaceId = InvalidPlace;
		}
		if ((flags & CA_MANIP) && m_manipPlaceId != InvalidPlace)
		{
			if (removeSwap(m_gameObject->m_constructionChagnedComponents, m_manipPlaceId))
				m_gameObject->m_constructionChagnedComponents[m_manipPlaceId]->m_manipPlaceId = m_manipPlaceId;
			m_manipPlaceId = InvalidPlace;
		}
	}

	void Component::setPriority(int32 priority) noexcept
	{
		m_priority = priority;
		GameObjectManager::instance().markComponentsDirty();
	}

	void Component::awake()
	{
	}

	void Component::sleep()
	{
	}

	void Component::initialize()
	{
	}

	void Component::fixedUpdate()
	{
	}

	void Component::transformChanged()
	{
	}

	void Component::update()
	{
	}

	void Component::componentManipulation()
	{
	}

	void Component::serialize(BinaryWriter* writer) const
	{
	}

	void Component::deserialize(BinaryReader* reader)
	{
	}

	Component::Component() :
		m_placeId(InvalidPlace),
		m_priority(0),
		m_transformPlaceId(InvalidPlace),
		m_manipPlaceId(InvalidPlace),
		m_gameObject(nullptr)
	{
		GameObjectManager::instance().markComponentsDirty();
	}

	Component::~Component()
	{
		deactivete(CA_ALL);
		GameObjectManager::instance().markComponentsDirty();
	}

	void Component::setType(TypeNameCC type)
	{
		m_types.push_back(type);
	}
}
