//
// YAMLWriter.h
// Vorb Engine
//
// Created by Cristian Zaloj on 21 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file YAMLWriter.h
 * @brief API for writing YAML data.
 */

#pragma once

#ifndef Vorb_YAMLWriter_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_YAMLWriter_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "YAMLNode.h"
#include "YAMLConverters.h"

namespace keg {
    /// Changes how the node hierarchy works in the writer
    enum class WriterParam {
        BEGIN_MAP, ///< The current node will become a mapped type
        END_MAP, ///< Stop the mapping stage
        BEGIN_SEQUENCE, ///< The current node will house an array
            END_SEQUENCE, ///< Stop adding array elements
            KEY, ///< A key in a map
            VALUE ///< A value in a map
    };

    /// Writes YAML data
    class YAMLWriter {
    public:
        /// Allocates the emitter
        YAMLWriter();
        /// Frees the emitter
        ~YAMLWriter();

        /// Emit a value at the current stage
        /// @tparam T: Type of value (must have NodeValueConverter<T> defined)
        /// @param value: Data to be written
        /// @return Self
        template<typename T>
        YAMLWriter& operator<< (T value) {
            NodeValueConverter<T>::write(m_emitter, value);
            return *this;
        }
        /// Change the state of the document and the node hierarchy
        /// @param param: Change to the YAML flow
        /// @return Self
        YAMLWriter& push(const WriterParam& param);

        /// @return The YAML data constructed up to this point
        const cString c_str() const;
    private:
        YAMLEmitter* m_emitter; ///< Pointer to emitter implementation
    };
}

#endif // !Vorb_YAMLWriter_h__
