//
// KegEnvironment.h
// Vorb Engine
//
// Created by Cristian Zaloj on 17 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file KegEnvironment.h
 * 
 * \brief 
 */

#pragma once

#ifndef Vorb_KegEnvironment_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_KegEnvironment_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "KegType.h"

namespace keg {
    class Type;
    class Enum;

#define KEG_BAD_TYPE_ID 0
    const i32 TYPE_NUM_PREDEFINED = 44;

    // An Environment Which Holds All The Types That Have Been Defined
    class Environment {
    public:
        Environment();

        // Add A Type To The Environment
        ui32 addType(const nString& name, Type* type);

        // Get A Type From A Name
        Type* getType(const nString& str) const {
            auto kt = _typesByName.find(str);
            if (kt != _typesByName.end()) return kt->second;
            else return nullptr;
        }
        // Get A Type From An ID
        Type* getType(const ui32& id) const {
            auto kt = _typesByID.find(id);
            if (kt != _typesByID.end()) return kt->second;
            else return nullptr;
        }

        // Add An Enum To The Environment
        ui32 addEnum(const nString& name, Enum* type);

        // Get An Enum From A Name
        Enum* getEnum(const nString& str) const {
            auto kt = _enumsByName.find(str);
            if (kt != _enumsByName.end()) return kt->second;
            else return nullptr;
        }
        // Get An Enum From An ID
        Enum* getEnum(const ui32& id) const {
            auto kt = _enumsByID.find(id);
            if (kt != _enumsByID.end()) return kt->second;
            else return nullptr;
        }
    private:
        // Auto-Incrementing ID Counter
        ui32 _uuid;

        // Basic Parseable Types
        Type _internalTypes[TYPE_NUM_PREDEFINED];

        // Type Dictionaries
        std::map<nString, Type*> _typesByName;
        std::map<ui32, Type*> _typesByID;

        // Enum Dictionaries
        std::map<nString, Enum*> _enumsByName;
        std::map<ui32, Enum*> _enumsByID;
    };

    // Get The Global Environment Of Custom Types
    Environment* getGlobalEnvironment();
}

#endif // !Vorb_KegEnvironment_h__
