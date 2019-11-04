#pragma once
#ifndef __RESOURCEOBJECT_H__
#define __RESOURCEOBJECT_H__

#include "./Core.h"

namespace ge
{
	class ResourceObject 
	{
	protected:
		mutable std::atomic_ptrdiff_t m_refCount;
	public:
		ResourceObject() :
			m_refCount(0)
		{}
		virtual ~ResourceObject() = default;

		virtual void addRef() const noexcept
		{
			m_refCount.fetch_add(1, std::memory_order_relaxed);
		}

		size_t refCount() const noexcept 
		{
			return m_refCount.load();
		}

		/**
		* @brief протзводит деинкрементацию счётчика ссылок на еденицу
		* @return true - если ресурс потерял все ссылки и его
		* требуется удалить.
		*/
		bool deincrement() const noexcept
		{
			return m_refCount.fetch_sub(1, std::memory_order_relaxed) < 1;
		}

		/**
		* @brief освобождает ресурс если количество ссылок на него равно еденице
		* Перезагружать только в том случаи, если this
		* после освобождения должен быть дополнителео
		* сохранён, или спецефически удалён.
		* 
		* для отдачи или удаления любых внутренних ресурсов
		* использовать деконструктор.
		*/
		virtual void release() const noexcept
		{			
			if (m_refCount.fetch_sub(1, std::memory_order_relaxed) < 1)
			{
				delete this;
			}
		}	
	};

	template<typename T>
	class RPtr 
	{
		T* m_ptr;
		friend class RPtr;
	public:
		constexpr operator T* () const noexcept 
		{
			return m_ptr;
		}

		constexpr RPtr() :
			m_ptr(nullptr)
		{}

		RPtr(const RPtr& ptr) noexcept :
			m_ptr(ptr.m_ptr)
		{
			if (m_ptr)
				m_ptr->addRef();
		}

		template<typename X>
		RPtr(const RPtr<X>& ptr) noexcept :
			m_ptr(static_cast<T*>(ptr.m_ptr))
		{
			if (m_ptr)
				m_ptr->addRef();
		}

		RPtr& operator=(const RPtr& ptr) noexcept
		{
			if (m_ptr)
				m_ptr->release();
			m_ptr = ptr.m_ptr;
			if (m_ptr)
				m_ptr->addRef();
			return *this;
		}

		template<typename X>
		RPtr& operator=(const RPtr<X>& ptr) noexcept
		{
			if (m_ptr)
				m_ptr->release();
			m_ptr = static_cast<T*>(ptr.m_ptr);
			if (m_ptr)
				m_ptr->addRef();
			return *this;
		}

		template<typename X>
		RPtr(X* ptr) noexcept :
			m_ptr(static_cast<T*>(ptr))
		{
			if (m_ptr)
				m_ptr->addRef();
		}

		constexpr RPtr(std::nullptr_t ptr) noexcept :
			m_ptr(nullptr)
		{}

		template<typename X>
		RPtr& operator=(X* ptr) noexcept
		{
			if (m_ptr)
				m_ptr->release();
			m_ptr = static_cast<T*>(ptr);
			if (m_ptr)
				m_ptr->addRef();
			return *this;
		}

		constexpr RPtr& operator=(std::nullptr_t ptr) noexcept
		{
			if (m_ptr)
				m_ptr->release();
			m_ptr = nullptr;
			return *this;
		}

		T* get() const noexcept 
		{
			return m_ptr;
		}

		operator bool() const noexcept 
		{
			return !!m_ptr;
		}

		T& operator*() const noexcept
		{
			return *m_ptr;
		}

		T* operator->() const noexcept
		{
			return m_ptr;
		}

		~RPtr() 
		{
			if (m_ptr)
				m_ptr->release();
		}
	};
}

#endif