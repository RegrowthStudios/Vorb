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
    assert(i >= 0 && i < 2);
    return data[i];
}

template<typename T>
inline typename const Matrix2<T>::ColType& Matrix2<T>::operator[](int i) const {
    assert(i >= 0 && i < 2);
    return data[i];
}

template<typename T>
inline Matrix2<T>::operator glm::tmat2x2<T>() const {
    return glm::tmat2x2<T>(c0[0], c0[1],
                           c1[0], c1[1]);
}

template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator=(const glm::tmat2x2<U>& rhs) {
    c0 = rhs[0];
    c1 = rhs[1];
}
template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator=(const Matrix2<U>& rhs) {
    c0 = rhs.c0;
    c1 = rhs.c1;
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
    c0 OP static_cast<T>(a); \
    c1 OP static_cast<T>(a); \
    return *this; \
} \
template<typename T> \
template<typename U> \
inline Matrix2<T>& Matrix2<T>::operator##OP##(const Matrix2<U>& m) { \
    c0 OP m[0]; \
    c1 OP m[1]; \
    return *this; \
}

// Add compound assignment operator code for Matrix2.
ADD_COMPOUND_ASSIGNMENT_OPERATORS;

template<typename T>
template<typename U>
inline Matrix2<T>& Matrix2<T>::operator*=(U a) {
    c0 *= a;
    c1 *= a;
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
    c0 /= a;
    c1 /= a;
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

#undef GLOBAL_OPERATOR

#pragma endregion Matrix2

/************************************************************************/
/* Matrix3 Implementation                                               */
/************************************************************************/
#pragma region Matrix3

template<typename T>
inline typename Matrix3<T>::ColType& Matrix3<T>::operator[](int i) {
    assert(i >= 0 && i < 3);
    return data[i];
}

template<typename T>
inline typename const Matrix3<T>::ColType& Matrix3<T>::operator[](int i) const {
    assert(i >= 0 && i < 3);
    return data[i];
}

template<typename T>
inline Matrix3<T>::operator glm::tmat3x3<T>() const {
    return glm::tmat3x3<T>(c0[0], c0[1], c0[2],
                           c1[0], c1[1], c1[2],
                           c2[0], c2[1], c2[2]);
}

template<typename T>
template<typename U>
inline Matrix3<T>& Matrix3<T>::operator=(const glm::tmat3x3<U>& rhs) {
    c0 = rhs[0];
    c1 = rhs[1];
    c2 = rhs[2];
}
template<typename T>
template<typename U>
inline Matrix3<T>& Matrix3<T>::operator=(const Matrix3<U>& rhs) {
    c0 = rhs.c0;
    c1 = rhs.c1;
    c2 = rhs.c2;
}
template<typename T>
inline bool Matrix3<T>::operator==(const Matrix3<T>& rhs) const {
    return (c0 == rhs.c0) && (c1 == rhs.c1) && (c2 == rhs.c2);
}
template<typename T>
inline bool Matrix3<T>::operator!=(const Matrix3<T>& rhs) const {
    return (c0 != rhs.c0) || (c1 != rhs.c1) || (c2 != rhs.c2);
}

// Code reduction for compound assignment operators.
#define COMPOUND_ASSIGNMENT(OP) \
template<typename T> \
template<typename U> \
inline Matrix3<T>& Matrix3<T>::operator##OP##(U a) { \
    c0 OP static_cast<T>(a); \
    c1 OP static_cast<T>(a); \
    c2 OP static_cast<T>(a); \
    return *this; \
} \
template<typename T> \
template<typename U> \
inline Matrix3<T>& Matrix3<T>::operator##OP##(const Matrix3<U>& m) { \
    c0 OP m[0]; \
    c1 OP m[1]; \
    c2 OP m[2]; \
    return *this; \
}

// Add compound assignment operator code for Matrix3.
ADD_COMPOUND_ASSIGNMENT_OPERATORS;

template<typename T>
template<typename U>
inline Matrix3<T>& Matrix3<T>::operator*=(U a) {
    c0 *= a;
    c1 *= a;
    c2 *= a;
    return *this;
}

template<typename T>
template<typename U>
inline Matrix3<T>& Matrix3<T>::operator*=(const Matrix3<U>& m) {
    return (*this = *this * m);
}

template<typename T>
template<typename U>
inline Matrix3<T>& Matrix3<T>::operator/=(U a) {
    data[0] /= a;
    data[1] /= a;
    data[2] /= a;
    return *this;
}

template<typename T>
template<typename U>
inline Matrix3<T>& Matrix3<T>::operator/=(const Matrix3<U>& m) {
    return (*this = *this * vmath::computeInverse(m));
}

#undef COMPOUND_ASSIGNMENT

// Code reduction for bitwise and arithmetic operators.
#define GLOBAL_OPERATOR(OP) \
template<typename T> \
inline Matrix3<T> operator##OP##(const Matrix3<T>& m, T a) { \
    return Matrix3<T>(m[0] ##OP## a, m[1] ##OP## a, m[2] ##OP## a); \
} \
template<typename T> \
inline Matrix3<T> operator##OP##(T a, const Matrix3<T>& m) { \
    return Matrix3<T>(a ##OP## m[0], a ##OP## m[1], a ##OP## m[2]); \
} \
template<typename T> \
inline Matrix3<T> operator##OP##(const Matrix3<T>& m1, const Matrix3<T>& m2) { \
    return Matrix3<T>(m1[0] ##OP## m2[0], m1[1] ##OP## m2[1], m1[2] ##OP## m2[2]); \
}

// Add global operator code for Matrix3.
ADD_GLOBAL_OPERATORS;

template<typename T>
inline Matrix3<T> operator*(const Matrix3<T>& m, T a) {
    return Matrix3<T>(m[0] * a, m[1] * a);
}
template<typename T>
inline Matrix3<T> operator*(T a, const Matrix3<T>& m) {
    return Matrix3<T>(a * m[0], a * m[1]);
}
template<typename T>
inline typename Matrix3<T>::ColType operator*(const Matrix3<T>& m, typename const Matrix3<T>::RowType& v) {
    return Matrix3<T>::ColType(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
                               m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
                               m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
}
template<typename T>
inline typename Matrix3<T>::RowType operator*(typename const Matrix3<T>::ColType& v, const Matrix3<T>& m) {
    return Matrix3<T>::RowType(v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2],
                               v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2],
                               v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2]);
}
template<typename T>
inline Matrix3<T> operator*(const Matrix3<T>& m1, const Matrix3<T>& m2) {
    return Matrix3<T>(m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
                      m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
                      m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
                      m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
                      m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
                      m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2],
                      m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
                      m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
                      m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2]);
}

template<typename T>
inline Matrix3<T> operator/(const Matrix3<T>& m, T a) {
    return Matrix3<T>(m[0] / a, m[1] / a, m[2] / a);
}
template<typename T>
inline Matrix3<T> operator/(T a, const Matrix3<T>& m) {
    return Matrix3<T>(a / m[0], a / m[1], a / m[2]);
}
template<typename T>
inline Matrix3<T> operator/(const Matrix3<T>& m1, const Matrix3<T>& m2) {
    return m1 * vmath::computeInverse(m2);
}
template<typename T>
inline typename Matrix3<T>::ColType operator/(const Matrix3<T>& m, typename const Matrix3<T>::RowType& v) {
    return vmath::computeInverse(m) * v;
}
template<typename T>
inline typename Matrix3<T>::RowType operator/(typename const Matrix3<T>::ColType& v, const Matrix3<T>& m) {
    return v * vmath::computeInverse(m);
}

#undef GLOBAL_OPERATOR

#pragma endregion Matrix3

#undef ADD_GLOBAL_OPERATORS
#undef ADD_COMPOUND_ASSIGNMENT_OPERATORS