/* This file implements all operators for the Vector types. */

// Helper macro to add assignment operators for the currently defined vector.
#define ADD_COMPOUND_ASSIGNMENT_OPERATORS \
COMPOUND_ASSIGNMENT(+= ); \
COMPOUND_ASSIGNMENT(-= ); \
COMPOUND_ASSIGNMENT(*= ); \
COMPOUND_ASSIGNMENT(/= ); \
COMPOUND_ASSIGNMENT(%= ); \
COMPOUND_ASSIGNMENT(&= ); \
COMPOUND_ASSIGNMENT(|= ); \
COMPOUND_ASSIGNMENT(^= ); \
COMPOUND_ASSIGNMENT(<<=); \
COMPOUND_ASSIGNMENT(>>=);

// Helper macro to add global operators for the currently defined vector.
#define ADD_GLOBAL_OPERATORS \
GLOBAL_OPERATOR(+ ); \
GLOBAL_OPERATOR(- ); \
GLOBAL_OPERATOR(* ); \
GLOBAL_OPERATOR(/ ); \
GLOBAL_OPERATOR(% ); \
GLOBAL_OPERATOR(& ); \
GLOBAL_OPERATOR(| ); \
GLOBAL_OPERATOR(^ ); \
GLOBAL_OPERATOR(<<); \
GLOBAL_OPERATOR(>>); 

/************************************************************************/
/* Vector2 Implementation                                               */
/************************************************************************/
#pragma region Vector2

template<typename T>
inline T& Vector2<T>::operator[](int i) {
    assert(i >= 0 && i < 2);
    return data[i];
}
template<typename T>
inline const T& Vector2<T>::operator[](int i) const {
    assert(i >= 0 && i < 2);
    return data[i];
}

template<typename T>
inline Vector2<T>::operator glm::tvec2<T>() const {
    return glm::tvec2<T>(x, y);
}

template<typename T>
template<typename U>
inline Vector2<T>& Vector2<T>::operator=(const glm::tvec2<U>& rhs) {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);
    return *this;
}
template<typename T>
template<typename U>
inline Vector2<T>& Vector2<T>::operator=(const Vector2<U>& rhs) {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);
    return *this;
}

template<typename T>
inline bool Vector2<T>::operator==(const Vector2<T>& rhs) const {
    return (x == rhs.x) && (y == rhs.y);
}
template<typename T>
inline bool Vector2<T>::operator!=(const Vector2<T>& rhs) const {
    return (x != rhs.x) || (y != rhs.y);
}

// Code reduction for compound assignment operators.
#define COMPOUND_ASSIGNMENT(OP) \
template<typename T> \
template<typename U> \
inline Vector2<T>& Vector2<T>::operator##OP##(U a) { \
    x OP static_cast<T>(a); \
    y OP static_cast<T>(a); \
    return *this; \
} \
template<typename T> \
template<typename U> \
inline Vector2<T>& Vector2<T>::operator##OP##(const Vector2<U>& v) { \
    x OP static_cast<T>(v.x); \
    y OP static_cast<T>(v.y); \
    return *this; \
}

// Add compound assignment operator code for Vector2.
ADD_COMPOUND_ASSIGNMENT_OPERATORS;

#undef COMPOUND_ASSIGNMENT

// Code reduction for bitwise and arithmetic operators.
#define GLOBAL_OPERATOR(OP) \
template<typename T> \
inline Vector2<T> operator##OP##(const Vector2<T>& v, T a) { \
    return Vector2<T>(v.x ##OP## a, v.y ##OP## a); \
} \
template<typename T> \
inline Vector2<T> operator##OP##(T a, const Vector2<T>& v) { \
    return Vector2<T>(a ##OP## v.x, a ##OP## v.y); \
} \
template<typename T> \
inline Vector2<T> operator##OP##(const Vector2<T>& v1, const Vector2<T>& v2) { \
    return Vector2<T>(v1.x ##OP## v2.x, v1.y ##OP## v2.y); \
}

// Add global operator code for Vector2.
ADD_GLOBAL_OPERATORS;

template<typename T>
inline Vector2<T> operator~(const Vector2<T>& v) {
    return Vector2<T>(~v.x, ~v.y); 
}

#undef GLOBAL_OPERATOR

#pragma endregion Vector2

/************************************************************************/
/* Vector3 Implementation                                               */
/************************************************************************/
#pragma region Vector3

template<typename T>
inline T& Vector3<T>::operator[](int i) {
    assert(i >= 0 && i < 3);
    return data[i];
}
template<typename T>
inline const T& Vector3<T>::operator[](int i) const {
    assert(i >= 0 && i < 3);
    return data[i];
}

template<typename T>
inline Vector3<T>::operator glm::tvec3<T>() const {
    return glm::tvec3<T>(x, y, z);
}

template<typename T>
template<typename U>
inline Vector3<T>& Vector3<T>::operator=(const glm::tvec3<U>& rhs) {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);
    z = static_cast<T>(rhs.z);
    return *this;
}
template<typename T>
template<typename U>
inline Vector3<T>& Vector3<T>::operator=(const Vector3<U>& rhs) {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);
    z = static_cast<T>(rhs.z);
    return *this;
}

template<typename T>
inline bool Vector3<T>::operator==(const Vector3<T>& rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}
template<typename T>
inline bool Vector3<T>::operator!=(const Vector3<T>& rhs) const {
    return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

// Code reduction for compound assignment operators.
#define COMPOUND_ASSIGNMENT(OP) \
template<typename T> \
template<typename U> \
inline Vector3<T>& Vector3<T>::operator##OP##(U a) { \
    x OP static_cast<T>(a); \
    y OP static_cast<T>(a); \
    z OP static_cast<T>(a); \
    return *this; \
} \
template<typename T> \
template<typename U> \
inline Vector3<T>& Vector3<T>::operator##OP##(const Vector3<U>& v) { \
    x OP static_cast<T>(v.x); \
    y OP static_cast<T>(v.y); \
    z OP static_cast<T>(v.z); \
    return *this; \
}

// Add compound assignment operator code for Vector3.
ADD_COMPOUND_ASSIGNMENT_OPERATORS;

#undef COMPOUND_ASSIGNMENT

// Code reduction for bitwise and arithmetic operators.
#define GLOBAL_OPERATOR(OP) \
template<typename T> \
inline Vector3<T> operator##OP##(const Vector3<T>& v, T a) { \
    return Vector3<T>(v.x ##OP## a, v.y ##OP## a, v.z ##OP## a); \
} \
template<typename T> \
inline Vector3<T> operator##OP##(T a, const Vector3<T>& v) { \
    return Vector3<T>(a ##OP## v.x, a ##OP## v.y, a ##OP## v.z); \
} \
template<typename T> \
inline Vector3<T> operator##OP##(const Vector3<T>& v1, const Vector3<T>& v3) { \
    return Vector3<T>(v1.x ##OP## v3.x, v1.y ##OP## v3.y, v1.z ##OP## v3.z); \
}

// Add global operator code for Vector3.
ADD_GLOBAL_OPERATORS;

template<typename T>
inline Vector3<T> operator~(const Vector3<T>& v) {
    return Vector3<T>(~v.x, ~v.y, ~v.z);
}

#undef GLOBAL_OPERATOR

#pragma endregion Vector3

/************************************************************************/
/* Vector4 Implementation                                               */
/************************************************************************/
#pragma region Vector4

template<typename T>
inline T& Vector4<T>::operator[](int i) {
    assert(i >= 0 && i < 4);
    return data[i];
}
template<typename T>
inline const T& Vector4<T>::operator[](int i) const {
    assert(i >= 0 && i < 4);
    return data[i];
}

template<typename T>
inline Vector4<T>::operator glm::tvec4<T>() const {
    return glm::tvec4<T>(x, y, z, w);
}

template<typename T>
template<typename U>
inline Vector4<T>& Vector4<T>::operator=(const glm::tvec4<U>& rhs) {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);
    z = static_cast<T>(rhs.z);
    w = static_cast<T>(rhs.w);
    return *this;
}
template<typename T>
template<typename U>
inline Vector4<T>& Vector4<T>::operator=(const Vector4<U>& rhs) {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);
    z = static_cast<T>(rhs.z);
    w = static_cast<T>(rhs.w);
    return *this;
}

template<typename T>
inline bool Vector4<T>::operator==(const Vector4<T>& rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}
template<typename T>
inline bool Vector4<T>::operator!=(const Vector4<T>& rhs) const {
    return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
}

// Code reduction for compound assignment operators.
#define COMPOUND_ASSIGNMENT(OP) \
template<typename T> \
template<typename U> \
inline Vector4<T>& Vector4<T>::operator##OP##(U a) { \
    x OP static_cast<T>(a); \
    y OP static_cast<T>(a); \
    z OP static_cast<T>(a); \
    w OP static_cast<T>(a); \
    return *this; \
} \
template<typename T> \
template<typename U> \
inline Vector4<T>& Vector4<T>::operator##OP##(const Vector4<U>& v) { \
    x OP static_cast<T>(v.x); \
    y OP static_cast<T>(v.y); \
    z OP static_cast<T>(v.z); \
    w OP static_cast<T>(v.w); \
    return *this; \
}

// Add compound assignment operator code for Vector4.
ADD_COMPOUND_ASSIGNMENT_OPERATORS;

#undef COMPOUND_ASSIGNMENT

// Code reduction for bitwise and arithmetic operators.
#define GLOBAL_OPERATOR(OP) \
template<typename T> \
inline Vector4<T> operator##OP##(const Vector4<T>& v, T a) { \
    return Vector4<T>(v.x ##OP## a, v.y ##OP## a, v.z ##OP## a, v.w ##OP## a); \
} \
template<typename T> \
inline Vector4<T> operator##OP##(T a, const Vector4<T>& v) { \
    return Vector4<T>(a ##OP## v.x, a ##OP## v.y, a ##OP## v.z, a ##OP## v.w); \
} \
template<typename T> \
inline Vector4<T> operator##OP##(const Vector4<T>& v1, const Vector4<T>& v4) { \
    return Vector4<T>(v1.x ##OP## v4.x, v1.y ##OP## v4.y, v1.z ##OP## v4.z, v1.w ##OP## v4.w); \
}

// Add global operator code for Vector4.
ADD_GLOBAL_OPERATORS;

template<typename T>
inline Vector4<T> operator~(const Vector4<T>& v) {
    return Vector4<T>(~v.x, ~v.y, ~v.z, ~v.w);
}

#undef GLOBAL_OPERATOR

#pragma endregion Vector4

#undef ADD_COMPOUND_ASSIGNMENT_OPERATORS
#undef ADD_GLOBAL_OPERATORS