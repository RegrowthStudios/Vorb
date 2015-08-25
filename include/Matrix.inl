/* This file implements all operators for the Matrix types. */
//#include "math/MatrixMath.h"

// Helper macro to add assignment operators for the currently defined vector.
#define ADD_COMPOUND_ASSIGNMENT_OPERATORS \
COMPOUND_ASSIGNMENT(+=); \
COMPOUND_ASSIGNMENT(-=);

// Helper macro to add global operators for the currently defined vector.
#define ADD_GLOBAL_OPERATORS \
GLOBAL_OPERATOR(+); \
GLOBAL_OPERATOR(-);

/************************************************************************/
/* Matrix2 Implementation                                               */
/************************************************************************/
#pragma region Matrix2

template<typename T>
inline typename Matrix2<T>::ColType& Matrix2<T>::operator[](int i) {
    return data[i];
}

template<typename T>
inline typename const Matrix2<T>::ColType& Matrix2<T>::operator[](int i) const {
    return data[i];
}

template<typename T>
inline Matrix2<T>::operator glm::tmat2x2<T>() const {
    return glm::tmat2x2<T>(data[0][0], data[0][1], data[1][0], data[1][1]);
}

template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator=(const glm::tmat2x2<U>& rhs) {
    data[0] = rhs[0];
    data[1] = rhs[1];
}
template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator=(const Matrix2<U>& rhs) {
    data[0] = rhs[0];
    data[1] = rhs[1];
}
template<typename T>
inline bool Matrix2<T>::operator==(const Matrix2<T>& rhs) const {
    return (data[0] == rhs.data[0]) && (data[1] == rhs.data[1]);
}
template<typename T>
inline bool Matrix2<T>::operator!=(const Matrix2<T>& rhs) const {
    return (data[0] != rhs.data[0]) || (data[1] != rhs.data[1]);
}

// Code reduction for compound assignment operators.
#define COMPOUND_ASSIGNMENT(OP) \
template<typename T> \
template<typename U> \
inline Matrix2<T>& Matrix2<T>::operator##OP##(U a) { \
    data[0] OP static_cast<T>(a); \
    data[1] OP static_cast<T>(a); \
    return *this; \
} \
template<typename T> \
template<typename U> \
inline Matrix2<T>& Matrix2<T>::operator##OP##(const Matrix2<U>& m) { \
    data[0] OP m[0]; \
    data[1] OP m[1]; \
    return *this; \
}

// Add compound assignment operator code for Matrix2.
ADD_COMPOUND_ASSIGNMENT_OPERATORS;

template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator*=(U a) {
    data[0] *= a;
    data[1] *= a;
    return *this;
}

template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator*=(const Matrix2<U>& m) {
    return (*this = *this * m);
}

template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator/=(U a) {
    data[0] /= a;
    data[1] /= a;
    return *this;
}

template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator/=(const Matrix2<U>& m) {
    return (*this = *this * vmath::computeInverse(m));
}

#undef COMPOUND_ASSIGNMENT

// Code reduction for bitwise and arithmetic operators.
#define GLOBAL_OPERATOR(OP) \
template<typename T> \
inline Matrix2<T> operator##OP##(const Matrix2<T>& m, T a) { \
    return Matrix2<T>(m[0] ##OP## a, m[1] ##OP## a); \
} \
template<typename T> \
inline Matrix2<T> operator##OP##(T a, const Matrix2<T>& m) { \
    return Matrix2<T>(a ##OP## m[0], a ##OP## m[1]); \
} \
template<typename T> \
inline Matrix2<T> operator##OP##(const Matrix2<T>& m1, const Matrix2<T>& m2) { \
    return Matrix2<T>(m1[0] ##OP## m2[0], m1[1] ##OP## m2[1]); \
}

// Add global operator code for Matrix2.
ADD_GLOBAL_OPERATORS;

template<typename T>
inline Matrix2<T> operator*(const Matrix2<T>& m, T a) {
    return Matrix2<T>(m[0] * a, m[1] * a);
}
template<typename T>
inline Matrix2<T> operator*(T a, const Matrix2<T>& m) {
    return Matrix2<T>(a * m[0], a * m[1]);
}
template<typename T>
inline typename Matrix2<T>::ColType operator*(const Matrix2<T>& m, typename const Matrix2<T>::RowType& v) {
    return Matrix2<T>::ColType(m[0][0] * v.x + m[1][0] * v.y,
                               m[0][1] * v.x + m[1][1] * v.y);
}
template<typename T>
inline typename Matrix2<T>::RowType operator*(typename const Matrix2<T>::ColType& v, const Matrix2<T>& m) {
    return Matrix2<T>::RowType(v.x * m[0][0] + v.y * m[0][1],
                               v.x * m[1][0] + v.y * m[1][1]);
}
template<typename T>
inline Matrix2<T> operator*(const Matrix2<T>& m1, const Matrix2<T>& m2) {
    return Matrix2<T>(m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                      m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                      m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                      m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1]);
}

template<typename T> 
inline Matrix2<T> operator/(const Matrix2<T>& m, T a) {
    return Matrix2<T>(m[0] / a, m[1] / a); 
} 
template<typename T>
inline Matrix2<T> operator/(T a, const Matrix2<T>& m) {
    return Matrix2<T>(a / m[0], a / m[1]);
} 
template<typename T>
inline Matrix2<T> operator/(const Matrix2<T>& m1, const Matrix2<T>& m2) {
    return m1 * vmath::computeInverse(m2);
}
template<typename T>
inline typename Matrix2<T>::ColType operator/(const Matrix2<T>& m, typename const Matrix2<T>::RowType& v) {
    return vmath::computeInverse(m) * v;
}
template<typename T>
inline typename Matrix2<T>::RowType operator/(typename const Matrix2<T>::ColType& v, const Matrix2<T>& m) {
    return v * vmath::computeInverse(m);
}

#undef ADD_GLOBAL_OPERATORS

#pragma endregion Matrix2
