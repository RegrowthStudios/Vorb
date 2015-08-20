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
inline Vector2<T>::operator glm::tvec2<T>() {
    glm::tvec2<T> rv;
    rv.x = x;
    rv.y = y;
    return rv;
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
    return (x == rhs.x && y == rhs.y);
}
template<typename T>
inline bool Vector2<T>::operator!=(const Vector2<T>& rhs) const {
    return (x != rhs.x || y != rhs.y);
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

COMPOUND_ASSIGNMENT(+=);
COMPOUND_ASSIGNMENT(-=);
COMPOUND_ASSIGNMENT(*=);
COMPOUND_ASSIGNMENT(/=);
COMPOUND_ASSIGNMENT(%=);
COMPOUND_ASSIGNMENT(&=);
COMPOUND_ASSIGNMENT(|=);
COMPOUND_ASSIGNMENT(^=);
COMPOUND_ASSIGNMENT(<<=);
COMPOUND_ASSIGNMENT(>>=);
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

GLOBAL_OPERATOR(+);
GLOBAL_OPERATOR(-);
GLOBAL_OPERATOR(*);
GLOBAL_OPERATOR(/ );
GLOBAL_OPERATOR(%);
GLOBAL_OPERATOR(&);
GLOBAL_OPERATOR(| );
GLOBAL_OPERATOR(^);
GLOBAL_OPERATOR(<< );
GLOBAL_OPERATOR(>> );
#undef STANDARD_OPERATOR

#pragma endregion Vector2