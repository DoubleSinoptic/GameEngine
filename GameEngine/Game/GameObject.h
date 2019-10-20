#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Core/Core.h"
#include "Core/Serializer.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Component.h"

namespace ge
{
	class GE_EXPORT GameObject : public ISerializable
	{
		friend class		Component;
		Vector<Component*>	m_transformChangedComponents;
		Vector<Component*>	m_constructionChagnedComponents;
		Vector<Component*>	m_components;
		Vector<GameObject*>	m_childrens;
		GameObject*			m_parent;
		String				m_name;
		Vector3				m_globlPosition;
		Quaternion			m_globlRotation;
		Vector3				m_localPosition;
		Quaternion			m_localRotation;
		usize				m_placeId;
	public:
	    GameObject* const* begin() const noexcept
		{
			return m_childrens.data();
		}

		GameObject* const* end() const noexcept
		{
			return m_childrens.data() + m_childrens.size();
		}

		GameObject** begin() noexcept
		{
			return m_childrens.data();
		}

		GameObject** end() noexcept
		{
			return m_childrens.data() + m_childrens.size();
		}

		GameObject();
		GameObject(const String& s);

		~GameObject();

		void setParent(GameObject* parent);

		constexpr GameObject* parent() const noexcept
		{
			return m_parent;
		}

		template<typename T>
		T* addComponent();

		template<typename T>
		T* getComponent() const;

		template<typename T>
		Vector<T*> getComponents() const;

		Vector3 up() const noexcept;

		Vector3 forward() const noexcept;

		Vector3 right() const noexcept;

		Vector3 localPosition() const noexcept;

		Quaternion localRotation() const noexcept;

		Vector3 position() const noexcept;

		Quaternion rotation() const noexcept;

		void setRotation(const Quaternion& rot) noexcept;

		void setPosition(const Vector3& pos) noexcept;

		void setTransfrom(const Vector3& pos, const Quaternion& rot) noexcept;

		void setLocalRotation(const Quaternion& rot) noexcept;

		void setLocalPosition(const Vector3& position) noexcept;

		void onTransformChanged();

		void onConstructionChanged();

		static void clearScene();
		static Vector<GameObject*> find(const String& expr);

		void serialize(BinaryWriter* writer) const override;
		void deserialize(BinaryReader* reader) override;
	};

	template<typename T>
	inline T* GameObject::addComponent()
	{
		Component* newComponent = new T();
		newComponent->setType(getTypeName<T>());
		newComponent->setGameObject(this);
		newComponent->initialize();
		m_components.push_back(newComponent);
		return static_cast<T*>(newComponent);
	}

	template<typename T>
	inline T* GameObject::getComponent() const
	{
		for (auto x : m_components) {
			if (x->is(getTypeName<T>()))
				return static_cast<T*>(x);
		}
		return nullptr;
	}

	template<typename T>
	inline Vector<T*> GameObject::getComponents() const
	{
		Vector<T*> components;
		for (auto x : m_components) {
			if (x->is(getTypeName<T>()))
				components.Add(static_cast<T*>(x));
		}
		return std::move(components);
	}
}

#endif