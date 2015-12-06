#include <immintrin.h>

static const f32 negOneHalf = -0.5f;

/// Class designed to use SSE when possible for:
///   fast flooring
///   fast ceiling
///   fast rounding toward zero
///   fast rounding away from zero
/// @tparam T: Input type: [f64, f32]
/// @tparam U: Output type: [f64, f32, i64, i32]
template<class T, class U>
class FastConversion {
public:
    /// Rounds to the next lowest whole number
    /// \code{.unparsed}
    /// floor(1.5)  -->  1.0
    /// floor(-1.5) --> -2.0
    /// \endcode
    inline static U floor(const T& x) {
        U output = 0;
        // slowest version
#if defined(__APPLE__) || defined(__linux__) || defined(WT64)
        output = static_cast<U>(std::floor(x));
#elif defined(OS_WINDOWS) && defined(ARCH_X86_32) && 0 
        __asm {
            fld x;
            fadd st, st(0);
            fadd negOneHalf;
            fistp i;
            sar i, 1;
        };
#else
        output = static_cast<U>(std::floor(x));
#endif
        return output;
    }
    /// Rounds to the next highest whole number
    /// \code{.unparsed}
    /// ceiling(1.5)  -->  2.0
    /// ceiling(-1.5) --> -1.0
    /// \endcode
    inline static U ceiling(const T& x) {
        U output = 0;
#if defined(__APPLE__) || defined(__linux__) || defined(WT64)
        output = static_cast<U>(std::ceil(x));
#elif defined(OS_WINDOWS) && defined(ARCH_X86_32) && 0 
        __asm {
            fld x;
            fadd st, st(0);
            fsubr negOneHalf;
            fistp i;
            sar i, 1;
        };
#else
        output = static_cast<U>(std::ceil(x));
#endif
        return output;
    }
    /// Rounds towards zero
    /// \code{.unparsed}
    /// trunc(1.5)  -->  1.0
    /// trunc(-1.5) --> -1.0
    /// \endcode
    inline static U trunc(const T& x) {
        U output = 0;
#if defined(__APPLE__) || defined(__linux__) || defined(WT64)
        output = static_cast<U>(std::trunc(x));
#elif defined(OS_WINDOWS) && defined(ARCH_X86_32)
        output = static_cast<U>(std::trunc(x));
#else
        output = static_cast<U>(std::trunc(x));
#endif
        return output;
    }
    /// Rounds to nearest
    /// \code{.unparsed}
    /// round(1.5)  -->  2.0
    /// round(1.4)  -->  1.0
    /// round(-1.4) --> -1.0
    /// round(-1.5) --> -2.0
    /// \endcode
    inline static U round(const T& x) {
        U output = 0;
#if defined(__APPLE__) || defined(__linux__) || defined(WT64)
        output = static_cast<U>(std::round(x));
#elif defined(OS_WINDOWS) && defined(ARCH_X86_32)
        output = static_cast<U>(std::round(x));
#else
        output = static_cast<U>(std::round(x));
#endif
        return output;
    }
};

/************************************************************************/
/* Floating-Integer Conversions                                         */
/************************************************************************/
inline i32 fastFloor(f64 x) {
    return FastConversion<f64, i32>::floor(x);
}
inline i32 fastFloor(f32 x) {
    return FastConversion<f32, i32>::floor(x);
}
inline i32 fastCeil(f64 x) {
    return FastConversion<f64, i32>::ceiling(x);
}
inline i32 fastCeil(f32 x) {
    return FastConversion<f32, i32>::ceiling(x);
}
