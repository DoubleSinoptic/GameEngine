#pragma once
#ifndef __ANY_H__
#define __ANY_H__

#include "./Core.h"

namespace ge
{
	struct AnyProxyBase
	{
		AnyProxyBase(const char* typeId) :
			typeName(typeId)
		{}
		const char* const typeName;
		virtual ~AnyProxyBase() = default;
		virtual AnyProxyBase* copy() const = 0;
	};

	template<typename T>
	struct AnyProxy final : public AnyProxyBase
	{
		template<typename X>
		AnyProxy(X&& v) :
			value(std::forward<X>(v)),
			AnyProxyBase(getTypeName<T>())
		{}
		T value;
		virtual AnyProxyBase* copy() const
		{
			return new AnyProxy<T>(value);
		}
	};

	class Any
	{
		AnyProxyBase* m_proxy;

		void tidy()
		{
			if (m_proxy) {
				delete m_proxy;
				m_proxy = nullptr;
			}
		}
	public:
		Any() noexcept :
			m_proxy(nullptr)
		{}

		template<
			typename T,
			typename = typename std::enable_if<!std::is_same<typename std::remove_reference<T>::type, Any>::value, void>::type >
			Any(T&& value) :
			m_proxy(new AnyProxy<typename std::decay<T>::type>(std::forward<T>(value)))
		{}

		bool hasValue() const noexcept
		{
			return m_proxy;
		}

		template<typename T>
		bool is() const
		{
			if (!m_proxy || !m_proxy->typeName)
				return false;
			return !strcmp(m_proxy->typeName, getTypeName<T>());
		}

		template<typename T>
		const T& get() const
		{
			if (!m_proxy || !m_proxy->typeName)
				geAssertFalse("invalid any container");
			if (!strcmp(m_proxy->typeName, getTypeName<T>()))
				return static_cast<AnyProxy<T> const*>(m_proxy)->value;
			geAssertFalse("invalid get type");
		}

		template<typename T>
		T& get()
		{
			if (!m_proxy || !m_proxy->typeName)
				geAssertFalse("invalid any container");
			if (!strcmp(m_proxy->typeName, getTypeName<T>()))
				return static_cast<AnyProxy<T>*>(m_proxy)->value;
			geAssertFalse("invalid get type");
		}

		Any(const Any& v) :
			m_proxy(v.m_proxy ? v.m_proxy->copy() : nullptr)
		{}

		Any& operator=(const Any& v)
		{
			tidy();
			m_proxy = v.m_proxy ? v.m_proxy->copy() : nullptr;
			return *this;
		}

		Any(Any&& v) noexcept :
			m_proxy(v.m_proxy)
		{
			v.m_proxy = nullptr;
		}

		Any& operator=(Any&& v) noexcept
		{
			tidy();
			std::swap(m_proxy, v.m_proxy);
			return *this;
		}

		~Any()
		{
			tidy();
		}
	};

}

#endif