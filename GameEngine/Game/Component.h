#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Core/Core.h"
#include "Core/Serializer.h"

namespace ge
{
	typedef const char* TypeNameCC;

	enum ComponentActivationFlags : uint32
	{
		CA_TRANSFORM = 1 << 0,
		CA_UPDATE = 1 << 1,
		CA_MANIP = 1 << 2,
		CA_ALL = 0xFF
	};
	
	class GameObject;
	class GE_EXPORT Component : public ISerializable
	{	
		Vector<TypeNameCC>			m_types;
		usize						m_placeId;
		usize						m_transformPlaceId;
		usize						m_manipPlaceId;
		int32						m_priority;
		GameObject*					m_gameObject;
	public:
		Component();

		virtual ~Component();

		void setType(TypeNameCC type);
		
		void activate(ComponentActivationFlags flags);

		void deactivete(ComponentActivationFlags flags);

		virtual void awake();

		virtual void sleep();

		virtual void initialize();

		/*unity like*/
		virtual void update();

		/*unity like*/
		virtual void fixedUpdate();

		virtual void transformChanged();

		virtual void componentManipulation();

	    bool is(TypeNameCC type) const noexcept
		{
			for (auto iType : m_types)
				if (!strcmp(type, iType))
					return true;
			return false;
		}

		constexpr void setPlace(usize index) 
		{
			m_placeId = index;
		}

		constexpr usize getPlace() const noexcept 
		{
			return m_placeId;
		}

		constexpr void setGameObject(GameObject* obj) noexcept
		{
			m_gameObject = obj;
		}

		void setPriority(int32 priority) noexcept;

		constexpr int32 getPriority() const noexcept 
		{
			return m_priority;
		}

		constexpr GameObject* getGameObject() const noexcept 
		{
			return m_gameObject;
		}

		virtual void serialize(BinaryWriter* writer) const override;
		virtual void deserialize(BinaryReader* reader) override;
	};
}

#endif