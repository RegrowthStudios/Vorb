//
// KegType.h
// Vorb Engine
//
// Created by Cristian Zaloj on 17 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file KegType.h
 * @brief 
 * 
 * 
 */

#pragma once

#ifndef Vorb_KegType_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegType_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>
#include "types.h"
#endif // !VORB_USING_PCH

#include "KegValue.h"

namespace keg {
    class Environment;
    class Value;

    // A Custom Data Type (Map Container Of Values)
    class Type {
        friend class Environment;
    public:
        // Create A Type With The Specified Size, Name, And Linked To An Optional Extra Environment
        Type(size_t sizeInBytes, const nString& name, Environment* env = nullptr);

        // Add A Value With A Key Attached To It (Values May Have More Than One Key)
        void addValue(const nString& name, const Value& type);

        // Only With Single-Inheritance And For The Brave. Would Not Recommend
        void addSuper(const Type* type, size_t offset = 0);

        // Attempt To Find A Value With
        const Value* getValue(const nString& name) const {
            auto kv = _values.find(name);
            if (kv != _values.end()) return &kv->second;
            else return nullptr;
        }

        // Value Traversal
        std::map<nString, Value>::const_iterator getIter() const {
            return _values.cbegin();
        }
        std::map<nString, Value>::const_iterator getIterEnd() const {
            return _values.cend();
        }

        // Set-Once Accessor
        size_t getSizeInBytes() const {
            return _sizeInBytes;
        }
        Type();

    private:
        size_t _sizeInBytes;
        std::map<nString, Value> _values;
    };
}

#endif // !Vorb_KegType_h__
