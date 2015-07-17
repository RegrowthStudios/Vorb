/*! \file typesArray.inl
 * @brief Bitfield implementation for different access methodologies.
 */

#define VORB_BITS_PER_BYTE 8

namespace vorb {
    /*! @brief
     */
    template<typename T, size_t BITOFF, size_t BITSIZE>
    class UncheckedBitfield {
        static_assert(std::is_integral<T>::value, "Bitfield may only represent integral types");
        static_assert(BITOFF < sizeof(T) * VORB_BITS_PER_BYTE, "Starting offset must be contained within the type");
        static_assert((BITOFF + BITSIZE) <= sizeof(T) * VORB_BITS_PER_BYTE, "Bitfield must be contained within the type");
    public:
        UncheckedBitfield& operator = (T v) {
            // Reduced by an extra masking operation for overflowss
            value = (value & ~(((1 << BITSIZE) - 1) << BITOFF)) | (v << BITOFF);
            return *this;
        }

        UncheckedBitfield& operator += (T v) {
            // Allows for overflows
            value += (v << BITOFF);
            return *this;
        }
        UncheckedBitfield& operator -= (T v) {
            // Allows for overflows
            value += (v << BITOFF);
            return *this;
        }
        UncheckedBitfield& operator *= (T v) {
            field *= v;
            return *this;
        }
        UncheckedBitfield& operator /= (T v) {
            field /= v;
            return *this;
        }
        UncheckedBitfield& operator |= (T v) {
            value |= (v << BITOFF);
            return *this;
        }
        UncheckedBitfield& operator &= (T v) {
            value &= (v << BITOFF) | ~(((1 << BITSIZE) - 1) << BITOFF);
            return *this;
        }
        UncheckedBitfield& operator ^= (T v) {
            value ^= (v << BITOFF);
            return *this;
        }

        T operator + (T v) {
            return field + v;
        }
        T operator - (T v) {
            return field - v;
        }
        T operator * (T v) {
            return field * v;
        }
        T operator / (T v) {
            return field / v;
        }
        T operator | (T v) {
            return field | v;
        }
        T operator & (T v) {
            return field & v;
        }
        T operator ^ (T v) {
            return field ^ v;
        }

        operator T() const {
            return field;
        }

        union {
            T value;
            struct {
                T : BITOFF;
                T field : BITSIZE;
                T : ((sizeof(T) * VORB_BITS_PER_BYTE) - (BITOFF + BITSIZE));
            };
        };
    };
}
