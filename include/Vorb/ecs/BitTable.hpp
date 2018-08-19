//
// BitTable.hpp
// Vorb Engine
//
// Created by Cristian Zaloj on 15 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file BitTable.hpp
 * @brief An M x N bit table for storing bools.
 */

#pragma once

#ifndef Vorb_BitTable_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_BitTable_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace ecs {
        class BitTable;

        /// Convenience class for accessing an array of truth values
        class BitArray {
            friend class BitTable;
        public:
            /// Empty constructor
            BitArray() {
                // Empty
            }

            /// Retrieve a bit value
            /// @param i: Index of value
            /// @return True if bit is non-zero
            bool valueOf(const ui32& i) {
                ui8& val = m_bits[i >> 3];
                return ((val >> (i & 0x07)) & 0x01) == 1;
            }

            /// Set a bit true
            /// @param i: Index of value
            void setTrue(const ui32& i) {
                ui8& val = m_bits[i >> 3];
                ui8 field = 0x01 << (i & 0x07);
                val |= field;
            }
            /// Set a bit false
            /// @param i: Index of value
            void setFalse(const ui32& i) {
                ui8& val = m_bits[i >> 3];
                ui8 field = 0x01 << (i & 0x07);
                val &= ~field;
            }
            /// Toggle a bit's value
            /// @param i: Index of value
            void toggleValue(const ui32& i) {
                ui8& val = m_bits[i >> 3];
                ui8 field = 0x01 << (i & 0x07);
                val ^= field;
            }
        private:
            /// Internal constructor
            /// @param bits: Data
            BitArray(ui8* bits) :
                m_bits(bits) {
                // Empty
            }

            ui8* m_bits = nullptr; ///< Pointer to bits
        };

        /// Table of bits stored in row-major order
        class BitTable {
        public:
            /// @ return Rows in the table
            const ui32& getRowCount() const {
                return m_rows;
            }
            /// @ return Columns in the table
            const ui32& getBitColumnCount() const {
                return m_columnsBits;
            }

            /// Obtain a row's bit data
            /// @param r: Row from which to obtain values
            /// @return Array pointer to the row (invalidates on this table's resizing operations)
            BitArray getRow(const ui32& r) {
                return BitArray(&m_bits[r * m_columns]);
            }

            /// Retrieve a bit value
            /// @param r: Row of value
            /// @param c: Column of value
            /// @return True if bit is non-zero
            bool valueOf(const ui32& r, const ui32& c) const {
                const ui8* bits = &m_bits[r * m_columns];
                bits += c >> 3;
                return ((*bits >> (c & 0x07)) & 0x01) == 1;
            }

            /// Set a bit true
            /// @param r: Row of value
            /// @param c: Column of value
            void setTrue(const ui32& r, const ui32& c) {
                ui8* val = &m_bits[r * m_columns];
                val += c >> 3;
                ui8 field = 0x01 << (c & 0x07);
                *val |= field;
            }
            /// Set a bit false
            /// @param r: Row of value
            /// @param c: Column of value
            void setFalse(const ui32& r, const ui32& c) {
                ui8* val = &m_bits[r * m_columns];
                val += c >> 3;
                ui8 field = 0x01 << (c & 0x07);
                *val &= ~field;
            }
            /// Toggle a bit's value
            /// @param r: Row of value
            /// @param c: Column of value
            void toggleValue(const ui32& r, const ui32& c) {
                ui8* val = &m_bits[r * m_columns];
                val += c >> 3;
                ui8 field = 0x01 << (c & 0x07);
                *val ^= field;
            }
            /// Clear out an entire row
            /// @param r: Row
            void setRowFalse(const ui32& r) {
                memset(m_bits.data() + r * m_columns, 0, m_columns);
            }

            /// Add columns to the table (resizes every 8 new columns)
            void addColumns(const size_t n) {
                m_columnsBits += n;
                ui32 col = (m_columnsBits + 7) >> 3;
                if (col != m_columns) {
                    if (m_rows > 0) {
                        m_bits.resize(col * m_rows);
                        ui32 diff = col - m_columns;

                        ui8* oldData = &m_bits[m_rows * m_columns - 1];
                        ui8* newData = &m_bits[m_rows * col - 1];     

                        // Translate the data
                        for (ui32 r = m_rows; r > 0;) {
                            r--;
                            for (ui32 c = 0; c < diff; c++) {
                                *newData = 0;
                                newData--;
                            }
                            for (ui32 c = 0; c < m_columns; c++) {
                                *newData = *oldData;
                                newData--;
                                oldData--;
                            }
                        }
                    }
                    m_columns = col;
                }
            }
            /// Add rows to the table
            void addRows(const size_t n) {
                m_bits.resize(m_bits.size() + n * m_columns);
                m_rows += n;
            }

        private:
            ui32 m_columnsBits = 0; ///< Number of columns (bits per row)
            ui32 m_columns = 0; ///< Number of columns (min. bytes per row)
            ui32 m_rows = 0; ///< Number of rows
            std::vector<ui8> m_bits; ///< Data
        };
    }
}
namespace vecs = vorb::ecs;

#endif // !Vorb_BitTable_hpp__
