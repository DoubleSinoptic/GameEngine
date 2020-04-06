#pragma once
#ifndef __SAMLLVECTOR_H__
#define __SAMLLVECTOR_H__

#include "Core.h"

namespace ge
{

	template<typename T, size_t length>
	class SmallVector {
		char m_data[length * sizeof(T)];
		size_t m_length;
	public:
		inline SmallVector(const SmallVector& v) :
			m_length(0),
			m_data{}
		{
			for (const auto& x : v)
				push_back(x);
		}

		inline SmallVector(SmallVector&& v) :
			m_length(0),
			m_data{}
		{
			for (auto& x : v)
				push_back(std::move(x));
			v.clear();
		}

		SmallVector& operator=(const SmallVector& v) {
			clear();
			for (const auto& x : v)
				push_back(x);
			return *this;
		}

		SmallVector& operator=(SmallVector&& v) {
			clear();
			for (auto& x : v)
				push_back(std::move(x));
			v.clear();
			return *this;
		}

		constexpr SmallVector() :
			m_length(0),
			m_data{}
		{}

		inline SmallVector(std::initializer_list<T> list) :
			m_length(0),
			m_data{}
		{
			for (const auto& x : list) {
				push_back(x);
			}
		}

		inline ~SmallVector() {
			clear();
		}

		inline void clear() {
			for (size_t i = 0; i < m_length; i++) {
				(reinterpret_cast<const T*>(m_data) + i)->~T();
			}
			m_length = 0;
		}

		inline void push_back(const T& v) {
			geAssert(m_length < length);
			new(begin() + m_length++) T(v);
		}

		inline void push_back(T&& v) {
			geAssert(m_length < length);
			new(begin() + m_length++) T(std::move(v));
		}

		template<typename... X>
		inline void emplace_back(X&&... v) {
			geAssert(m_length < length);
			new(begin() + m_length++) T(std::forward<X>(v)...);
		}

		constexpr const T& operator[](size_t index) const {
			return reinterpret_cast<const T*>(m_data)[index];
		}

		constexpr T& operator[](size_t index) {
			return reinterpret_cast<T*>(m_data)[index];
		}

		constexpr size_t size() const {
			return m_length;
		}

		constexpr const T* begin() const {
			return reinterpret_cast<const T*>(m_data);
		}

		constexpr const T* end() const {
			return reinterpret_cast<const T*>(m_data) + m_length;
		}

		constexpr T* begin() {
			return reinterpret_cast<T*>(m_data);
		}

		constexpr T* end() {
			return reinterpret_cast<T*>(m_data) + m_length;
		}

		const T* data() const {
			return reinterpret_cast<const T*>(m_data);
		}

		T* data() {
			return reinterpret_cast<T*>(m_data);
		}
	};

}

#endif