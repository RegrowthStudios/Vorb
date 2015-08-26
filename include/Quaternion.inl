/* This file implements all functions for Quaternion. */

template<typename T>
Quaternion<T>::Quaternion(const Vector3<T>& euler) {
    const Vector3<T> c = vmath::cos(euler * static_cast<T>(0.5));
    const Vector3<T> s = vmath::sin(euler * static_cast<T>(0.5));

    w = c.x * c.y * c.z + s.x * s.y * s.z;
    x = s.x * c.y * c.z - c.x * s.y * s.z;
    y = c.x * s.y * c.z + s.x * c.y * s.z;
    z = c.x * c.y * s.z - s.x * s.y * c.z;
}
template<typename T>
Quaternion<T>::Quaternion(const glm::tvec3<T>& euler) {
    const Vector3<T> e2(euler);
    const Vector3<T> c = vmath::cos(e2 * static_cast<T>(0.5));
    const Vector3<T> s = vmath::sin(e2 * static_cast<T>(0.5));

    w = c.x * c.y * c.z + s.x * s.y * s.z;
    x = s.x * c.y * c.z - c.x * s.y * s.z;
    y = c.x * s.y * c.z + s.x * c.y * s.z;
    z = c.x * c.y * s.z - s.x * s.y * c.z;
}

template<typename T>
inline T& Quaternion<T>::operator[](int i) {
    assert(i >= 0 && i < 3);
    return data[i];
}
template<typename T>
inline const T& Quaternion<T>::operator[](int i) const {
    assert(i >= 0 && i < 3);
    return data[i];
}

template<typename T>
template<typename U>
inline Quaternion<T>& Quaternion<T>::operator=(const Quaternion<U>& rhs) {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);
    z = static_cast<T>(rhs.z);
    w = static_cast<T>(rhs.w);
}

template<typename T>
inline bool Quaternion<T>::operator==(const Quaternion<T>& rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}
template<typename T>
inline bool Quaternion<T>::operator!=(const Quaternion<T>& rhs) const {
    return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
}

template<typename T>
template<typename U>
inline Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<U>& q) {
    x += static_cast<T>(q.x);
    y += static_cast<T>(q.y);
    z += static_cast<T>(q.z);
    w += static_cast<T>(q.w);
    return *this;
}

template<typename T>
template<typename U>
inline Quaternion<T>& Quaternion<T>::operator-=(const Quaternion<U>& q) {
    x -= static_cast<T>(q.x);
    y -= static_cast<T>(q.y);
    z -= static_cast<T>(q.z);
    w -= static_cast<T>(q.w);
    return *this;
}

template<typename T>
template<typename U>
inline Quaternion<T>& Quaternion<T>::operator*=(U a) {
    x *= static_cast<T>(a);
    y *= static_cast<T>(a);
    z *= static_cast<T>(a);
    w *= static_cast<T>(a);
    return *this;
}
template<typename T>
template<typename U>
inline Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<U>& q) {
    const Quaternion<T> p(*this);
    const Quaternion<T> cq(q); // Cast everything

    x = p.w * cq.x + p.x * cq.w + p.y * cq.z - p.z * cq.y;
    y = p.w * cq.y + p.y * cq.w + p.z * cq.x - p.x * cq.z;
    z = p.w * cq.z + p.z * cq.w + p.x * cq.y - p.y * cq.x;
    w = p.w * cq.w - p.x * cq.x - p.y * cq.y - p.z * cq.z;
    return *this;
}

template<typename T>
template<typename U>
inline Quaternion<T>& Quaternion<T>::operator/=(U a) {
    x /= static_cast<T>(a);
    y /= static_cast<T>(a);
    z /= static_cast<T>(a);
    w /= static_cast<T>(a);
    return *this;
}

/* Global operators */

template<typename T>
inline Quaternion<T> operator-(const Quaternion<T>& q) {
    return Quaternion<T>(-q.x, -q.y, -q.z, - q.w);
}

template<typename T>
inline Quaternion<T> operator+(const Quaternion<T>& q, const Quaternion<T>& p) {
    return Quaternion<T>(q) += p;
}

template<typename T>
inline Quaternion<T> operator*(const Quaternion<T>& q, const Quaternion<T>& p) {
    return Quaternion<T>(q) *= p;
}

template<typename T>
inline Vector3<T> operator*(const Quaternion<T>& q, const Vector3<T>& v) {
    const Vector3<T> qv(q.x, q.y, q.z);
    const Vector3<T> uv(vmath::cross(qv, v));
    const Vector3<T>uuv(vmath::cross(qv, uv));

    return v + ((uv * q.w) + uuv) * static_cast<T>(2);
}

template<typename T>
inline Vector3<T> operator*(const Vector3<T>& v, const Quaternion<T>& q) {
    return vmath::inverse(q) * v;
}

template<typename T>
inline Vector4<T> operator*(const Quaternion<T>& q, const Vector4<T>& v) {
    return Vector4<T>(q * Vector4<T>(v), v.w);
}

template<typename T>
inline Vector4<T> operator*(const Vector4<T>& v, const Quaternion<T>& q) {
    return vmath::inverse(q) * v;
}

template<typename T>
inline Quaternion<T> operator*(Quaternion<T> const & q, T const & s) {
    return Quaternion<T>(q.x * s, q.y * s, q.z * s, q.w * s);
}

template<typename T>
inline Quaternion<T> operator*(T const & s, Quaternion<T> const & q) {
    return q * s;
}

template<typename T>
inline Quaternion<T> operator/(Quaternion<T> const & q, T const & s) {
    return Quaternion<T>(q.x / s, q.y / s, q.z / s, q.w / s);
}
