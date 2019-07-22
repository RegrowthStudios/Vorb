//
// KegType.h
// Vorb Engine
//
// Created by Cristian Zaloj on 17 Feb 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file KegType.h
 * @brief 
 */

#pragma once

#ifndef Vorb_KegType_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegType_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>

#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "KegValue.h"
#include "../Event.hpp"

#define offsetOf(Class, Member) ((char*)&((Class*)nullptr)->Member - (char*)nullptr)

namespace keg {
    class Environment;
    struct Value;

    typedef void(*ConstructorFunction)(void*);
    typedef void* (*AllocatorFunction)();
    typedef void* (*AllocatorArrayFunction)(size_t s);
    typedef void (*DeallocatorFunction)(void* p);

    /*! @brief Custom YAML-mapped data type
     */
    class Type {
        friend class Environment;
    public:
        // Create A Type With The Specified Size, Name, And Linked To An Optional Extra Environment
        Type();
        Type(const nString& name, Environment* env = nullptr);

        // Add A Value With A Key Attached To It (Values May Have More Than One Key)
        void addValue(const nString& name, const Value& type);

        // Only With Single-Inheritance And For The Brave. Would Not Recommend
        void addSuper(const Type* type, size_t offset = 0);

        // Attempt To Find A Value With
        const Value* getValue(const nString& name) const {
            auto kv = m_values.find(name);
            if (kv != m_values.end()) return &kv->second;
            else return nullptr;
        }

        // Value Traversal
        std::map<nString, Value>::const_iterator getIter() const {
            return m_values.cbegin();
        }
        std::map<nString, Value>::const_iterator getIterEnd() const {
            return m_values.cend();
        }

        void construct(void* data);
        void* alloc();
        void* allocArray(size_t s);
        DeallocatorFunction getDeallocator() const;

        // Set-Once Accessor
        size_t getSizeInBytes() const {
            return m_sizeInBytes;
        }
        template<typename T>
        void setStructType() {
            m_sizeInBytes = sizeof(T);
            m_ctor = simpleCTOR<T>;
            m_alloc = simpleNew<T>;
            m_allocArray = simpleNewArray<T>;
            m_deallocArray = simpleDeleteArray<T>;
        }

        template<typename T>
        static void simpleCTOR(void* data) {
            new (data) T();
        }
        template<typename T>
        static void* simpleNew() {
            return new T();
        }
        template<typename T>
        static void* simpleNewArray(size_t s) {
            return new T[s]();
        }
        template<typename T>
        static void simpleDeleteArray(void* p) {
            T* ref = (T*)p;
            delete[] ref;
        }
    private:
        size_t m_sizeInBytes;
        std::map<nString, Value> m_values;
        ConstructorFunction m_ctor; ///< Constructor function
        AllocatorFunction m_alloc;
        AllocatorArrayFunction m_allocArray;
        DeallocatorFunction m_deallocArray;
    };
}

#endif // !Vorb_KegType_h__
