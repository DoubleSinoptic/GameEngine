#pragma once

#include "../Core/Core.h"

namespace ge
{
    class IContextObject {
        std::atomic_intptr_t m_refCounter;
        std::atomic_intptr_t m_temp;
        int m_age;
    public:
        virtual ~IContextObject() = default;
        IContextObject() :
            m_refCounter(0),
            m_age(0),
            m_temp(0)
        {}

        void AddRef() {
            m_refCounter++;
        }

        void Release() {
            if (--m_refCounter < 1)
                delete this;
        }

        intptr_t refCount() const {
            return m_refCounter.load();
        }

        constexpr IContextObject* contextObject() {
            return this;
        }

        void resetAge() {
            m_age = 0;
        }

        void addAge() {
            m_age += 1;
        }

        int age() const {
            return m_age;
        }
        
        void addTmp() {
            m_temp++;
        }

        void releaseTmp() {
            m_temp--;
        }
        
        intptr_t tmpCount() const {
            return m_temp.load();
        }
    };

    template<typename T>
    class CmPtr {
        T* m_obj;
    public:
        constexpr operator T* () const noexcept {
            return m_obj;
        }

        CmPtr(T* obj) noexcept :
            m_obj(obj)
        {
            if (obj)
                obj->AddRef();
        }

        constexpr CmPtr() noexcept :
            m_obj(nullptr)
        { }

        CmPtr(const CmPtr& ptr) noexcept :
            m_obj(ptr.m_obj)
        {
            if (ptr.m_obj)
                ptr.m_obj->AddRef();
        }

        CmPtr(CmPtr&& ptr) noexcept :
            m_obj(ptr.m_obj)
        {
            ptr.m_obj = nullptr;
        }

        ~CmPtr()
        {
            if (m_obj) {
                m_obj->Release();
                //m_obj = nullptr;
            }
        }

        CmPtr& operator=(T* obj) noexcept
        {
            if (m_obj) {
                m_obj->Release();
            }
            m_obj = obj;
            if (obj) {
                obj->AddRef();
            }
            return *this;
        }

        CmPtr& operator=(const CmPtr& ptr) noexcept
        {
            if (m_obj) {
                m_obj->Release();
            }
            m_obj = ptr.m_obj;
            if (ptr.m_obj) {
                ptr.m_obj->AddRef();
            }
            return *this;
        }

        CmPtr& operator=(CmPtr&& ptr) noexcept
        {
            if (m_obj) {
                m_obj->Release();
            }
            m_obj = ptr.m_obj;
            ptr.m_obj = nullptr;
            return *this;
        }

        /*not const T, T is T*/
        constexpr T* get() const {
            return m_obj;
        }  
        

        void** ppvtr() {
            return reinterpret_cast<void**>(&m_obj);
        }

        T** pptr() {
            return &m_obj;
        }

        void Repair() {
            if (m_obj) {
                m_obj->AddRef();
            }
        }
        constexpr T* operator ->() const {
            return m_obj;
        }
    };

    typedef CmPtr<IContextObject> IContextObjectRef;

    template<typename T, typename X>
    CmPtr<T> ref_static_cast(CmPtr<X> v) {
        return CmPtr<T>(static_cast<T*>(v.get()));
    }
}