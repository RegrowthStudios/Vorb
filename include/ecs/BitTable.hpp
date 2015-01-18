///
/// BitTable.hpp
/// Vorb Engine
///
/// Created by Cristian Zaloj on 15 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// An M x N bit table for storing bools
///

#pragma once

#ifndef BitTable_hpp__
#define BitTable_hpp__

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
                return ((val >> (i & 0x08)) & 0x01) == 1;
            }

            /// Set a bit true
            /// @param i: Index of value
            void setTrue(const ui32& i) {
                ui8& val = m_bits[i >> 3];
                ui8 field = 0x01 << (i & 0x08);
                val |= field;
            }
            /// Set a bit false
            /// @param i: Index of value
            void setFalse(const ui32& i) {
                ui8& val = m_bits[i >> 3];
                ui8 field = 0x01 << (i & 0x08);
                val &= ~field;
            }
            /// Toggle a bit's value
            /// @param i: Index of value
            void toggleValue(const ui32& i) {
                ui8& val = m_bits[i >> 3];
                ui8 field = 0x01 << (i & 0x08);
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
                return ((*bits >> (c & 0x08)) & 0x01) == 1;
            }

            /// Set a bit true
            /// @param r: Row of value
            /// @param c: Column of value
            void setTrue(const ui32& r, const ui32& c) {
                ui8* val = &m_bits[r * m_columns];
                val += c >> 3;
                ui8 field = 0x01 << (c & 0x08);
                *val |= field;
            }
            /// Set a bit false
            /// @param r: Row of value
            /// @param c: Column of value
            void setFalse(const ui32& r, const ui32& c) {
                ui8* val = &m_bits[r * m_columns];
                val += c >> 3;
                ui8 field = 0x01 << (c & 0x08);
                *val &= ~field;
            }
            /// Toggle a bit's value
            /// @param r: Row of value
            /// @param c: Column of value
            void toggleValue(const ui32& r, const ui32& c) {
                ui8* val = &m_bits[r * m_columns];
                val += c >> 3;
                ui8 field = 0x01 << (c & 0x08);
                *val ^= field;
            }
            /// Clear out an entire row
            /// @param r: Row
            void setRowFalse(const ui32& r) {
                ui8* val = &m_bits[r * m_columns];
                for (ui32 c = 0; c < m_columns; c++) {
                    *val = 0;
                    val++;
                }
            }

            /// Add a column to the table (resizes every 8 additions)
            void addColumn() {
                m_columnsBits++;
                ui32 col = (m_columnsBits + 7) >> 3;
                if (col != m_columns && m_rows > 0) {
                    m_bits.resize(col * m_rows);
                    ui32 diff = col - m_columns;

                    ui8* oldData = &m_bits[m_rows * m_columns];
                    oldData--;
                    ui8* newData = &m_bits[m_rows * col];
                    newData--;

                    // Translate the data
                    // TODO: Can this be made faster?
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

                    m_columns = col;
                }
            }
            /// Add a row to the table
            void addRow() {
                // Add a bunch of columns
                for (ui32 i = 0; i < m_columns; i++) m_bits.emplace_back();
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

#endif // BitTable_hpp__
