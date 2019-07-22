//
// KegEnum.h
// Vorb Engine
//
// Created by Cristian Zaloj on 17 Feb 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file KegEnum.h
 * @brief 
 */

#pragma once

#ifndef Vorb_KegEnum_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegEnum_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>

#include "Vorb/types.h"
#endif // !VORB_USING_PCH

namespace keg {
    class Environment;

#define KEG_ENUM_MAX_SIZE_IN_BYTES 8
    typedef ui64 EnumType;
    class Enum {
        friend class Environment;
    public:
        Enum(size_t sizeInBytes, const nString& name, Environment* env = nullptr);

        template<typename T>
        void addValue(const nString& s, T v) {
            static_assert(sizeof(T) <= sizeof(EnumType), "Value type to be added exceeds maximum size");
            EnumType eValue = static_cast<EnumType>(v);
            _values[s] = eValue;
            _valuesRev[eValue] = s;
        }
        void setValue(void* data, const nString& s) {
            _fSetter(data, s, this);
        }
        nString getValue(const void* data) {
            return _fGetter(data, this);
        }

        // Value Traversal
        std::map<nString, EnumType>::const_iterator getIter() const {
            return _values.cbegin();
        }
        std::map<nString, EnumType>::const_iterator getIterEnd() const {
            return _values.cend();
        }

        // Set-Once Accessor
        size_t getSizeInBytes() const {
            return _sizeInBytes;
        }
    private:
        template<typename T>
        static void setValue(void* data, const nString& s, Enum* e) {
            auto kv = e->_values.find(s);
            if (kv != e->_values.end()) *((T*)data) = static_cast<T>(kv->second);
        }
        template<typename T>
        static nString getValue(const void* data, const Enum* e) {
            T key = *((const T*)data);
            auto kv = e->_valuesRev.find(static_cast<EnumType>(key));
            return kv != e->_valuesRev.end() ? kv->second : "";
        }

        void(*_fSetter)(void*, const nString&, Enum*);
        nString(*_fGetter)(const void*, const Enum*);

        size_t _sizeInBytes;
        std::map<nString, EnumType> _values;
        std::map<EnumType, nString> _valuesRev;
    };
}

#endif // !Vorb_KegEnum_h__
