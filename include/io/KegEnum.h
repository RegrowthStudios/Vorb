//
// KegEnum.h
// Vorb Engine
//
// Created by Cristian Zaloj on 17 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file KegEnum.h
 * @brief 
 * 
 * 
 */

#pragma once

#ifndef Vorb_KegEnum_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegEnum_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>
#include "types.h"
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
            _values[s] = (EnumType)v;
            _valuesRev[(EnumType)v] = s;
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
        static void setValue64(void* data, const nString& s, Enum* e);
        static void setValue32(void* data, const nString& s, Enum* e);
        static void setValue16(void* data, const nString& s, Enum* e);
        static void setValue8(void* data, const nString& s, Enum* e);
        static nString getValue64(const void* data, Enum* e);
        static nString getValue32(const void* data, Enum* e);
        static nString getValue16(const void* data, Enum* e);
        static nString getValue8(const void* data, Enum* e);

        void(*_fSetter)(void*, const nString&, Enum*);
        nString(*_fGetter)(const void*, Enum*);

        size_t _sizeInBytes;
        std::map<nString, EnumType> _values;
        std::map<EnumType, nString> _valuesRev;
    };
}

#endif // !Vorb_KegEnum_h__
