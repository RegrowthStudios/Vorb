//
// Implementation of N4562 std::experimental::any (merged into C++17) for C++11 compilers.
//
// See also:
//   + http://en.cppreference.com/w/cpp/any
//   + http://en.cppreference.com/w/cpp/experimental/any
//   + http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4562.html#any
//   + https://cplusplus.github.io/LWG/lwg-active.html#2509
//
//
// Copyright (c) 2016 Denilson das Mercês Amorim
// Modified by Matthew Marshall, March 2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef Vorb_Any_hpp__
#define Vorb_Any_hpp__

#if !defined(_MSC_FULL_VER) || (_MSC_FULL_VER > 189999999)
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#include <new>
#include <stdexcept>
#include <typeinfo>
#include <type_traits>

class bad_any_cast : public std::bad_cast {
public:
    const char* what() const NOEXCEPT override {
        return "bad any cast";
    }
};

#ifndef DEFAULT_ANY_STACK_SIZE_MULT
#define DEFAULT_ANY_STACK_SIZE_MULT 4
#endif
template<size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
class basic_any final {
public:
    /// Constructs an object of type basic_any with an empty state.
    basic_any() :
        vtable(nullptr)
    {
    }

    /// Constructs an object of type basic_any with an equivalent state as other.
    basic_any(const basic_any& rhs) :
        vtable(rhs.vtable)
    {
        if (!rhs.empty()) {
            rhs.vtable->copy(rhs.storage, this->storage);
        }
    }

    /// Constructs an object of type basic_any with a state equivalent to the original state of other.
    /// rhs is left in a valid but otherwise unspecified state.
    basic_any(basic_any&& rhs) NOEXCEPT :
    vtable(rhs.vtable)
    {
        if (!rhs.empty()) {
            rhs.vtable->move(rhs.storage, this->storage);
            rhs.vtable = nullptr;
        }
    }

    /// Same effect as this->clear().
    ~basic_any() {
        this->clear();
    }


    /// Constructs an object of type basic_any that contains an object of type T stored by-copy.
    ///
    /// T shall satisfy the CopyConstructible requirements, otherwise the program is ill-formed.
    /// This is because an `basic_any` may be copy constructed into another `basic_any` at any time, so a copy should always be allowed.
    template<typename ValueType, typename = typename std::enable_if<!std::is_same<typename std::decay<ValueType>::type, basic_any>::value>::type>
    basic_any(const ValueType& value) {
        static_assert(std::is_copy_constructible<typename std::decay<ValueType>::type>::value,
            "T shall satisfy the CopyConstructible requirements.");
        this->construct(value);
    }
    /// Constructs an object of type basic_any that contains an object of type T direct-initialized with std::forward<ValueType>(value).
    ///
    /// T shall satisfy the CopyConstructible requirements, otherwise the program is ill-formed.
    /// This is because an `basic_any` may be copy constructed into another `basic_any` at any time, so a copy should always be allowed.
    template<typename ValueType, typename = typename std::enable_if<!std::is_same<typename std::decay<ValueType>::type, basic_any>::value>::type>
    basic_any(ValueType&& value) {
        static_assert(std::is_copy_constructible<typename std::decay<ValueType>::type>::value,
            "T shall satisfy the CopyConstructible requirements.");
        this->construct(std::forward<ValueType>(value));
    }

    /// Has the same effect as basic_any(rhs).swap(*this). No effects if an exception is thrown.
    basic_any& operator=(const basic_any& rhs) {
        basic_any(rhs).swap(*this);
        return *this;
    }

    /// Has the same effect as basic_any(std::move(rhs)).swap(*this).
    ///
    /// The state of *this is equivalent to the original state of rhs and rhs is left in a valid
    /// but otherwise unspecified state.
    basic_any& operator=(basic_any&& rhs) NOEXCEPT {
        basic_any(std::move(rhs)).swap(*this);
        return *this;
    }

    /// Has the same effect as basic_any(std::forward<ValueType>(value)).swap(*this). No effect if a exception is thrown.
    ///
    /// T shall satisfy the CopyConstructible requirements, otherwise the program is ill-formed.
    /// This is because an `basic_any` may be copy constructed into another `basic_any` at any time, so a copy should always be allowed.
    template<typename ValueType, typename = typename std::enable_if<!std::is_same<typename std::decay<ValueType>::type, basic_any>::value>::type>
    basic_any& operator=(ValueType&& value) {
        static_assert(std::is_copy_constructible<typename std::decay<ValueType>::type>::value,
            "T shall satisfy the CopyConstructible requirements.");
        any(std::forward<ValueType>(value)).swap(*this);
        return *this;
    }

    /// If not empty, destroys the contained object.
    void clear() NOEXCEPT {
        if (!empty()) {
            this->vtable->destroy(storage);
            this->vtable = nullptr;
        }
    }

    /// Returns true if *this has no contained object, otherwise false.
    bool empty() const NOEXCEPT {
        return this->vtable == nullptr;
    }

    /// If *this has a contained object of type T, typeid(T); otherwise typeid(void).
    const std::type_info& type() const NOEXCEPT {
        return empty() ? typeid(void) : this->vtable->type();
    }

    /// Exchange the states of *this and rhs.
    void swap(basic_any& rhs) NOEXCEPT
    {
        if (this->vtable != rhs.vtable) {
            basic_any tmp(std::move(rhs));

            // move from *this to rhs.
            rhs.vtable = this->vtable;
            if (this->vtable != nullptr) {
                this->vtable->move(this->storage, rhs.storage);
                //this->vtable = nullptr; -- uneeded, see below
            }

            // move from tmp (previously rhs) to *this.
            this->vtable = tmp.vtable;
            if (tmp.vtable != nullptr) {
                tmp.vtable->move(tmp.storage, this->storage);
                tmp.vtable = nullptr;
            }
        } else /* same types */ {
            if (this->vtable != nullptr)
                this->vtable->swap(this->storage, rhs.storage);
        }
    }

private: // Storage and Virtual Method Table
    template<typename StackStorage = typename std::aligned_storage<StackSizeMult * sizeof(void*), std::alignment_of<void*>::value>::type>
    union storage_union {
        using stack_storage_t = StackStorage;

        void*               dynamic;
        StackStorage        stack;      // 2 words for e.g. shared_ptr
    };

    /// Base VTable specification.
    struct vtable_type {
        // Note: The caller is responssible for doing .vtable = nullptr after destructful operations
        // such as destroy() and/or move().

        /// The type of the object this vtable is for.
        const std::type_info& (*type)() NOEXCEPT;

        /// Destroys the object in the union.
        /// The state of the union after this call is unspecified, caller must ensure not to use src anymore.
        void(*destroy)(storage_union<>&) NOEXCEPT;

        /// Copies the **inner** content of the src union into the yet unitialized dest union.
        /// As such, both inner objects will have the same state, but on separate memory locations.
        void(*copy)(const storage_union<>& src, storage_union<>& dest);

        /// Moves the storage from src to the yet unitialized dest union.
        /// The state of src after this call is unspecified, caller must ensure not to use src anymore.
        void(*move)(storage_union<>& src, storage_union<>& dest) NOEXCEPT;

        /// Exchanges the storage between lhs and rhs.
        void(*swap)(storage_union<>& lhs, storage_union<>& rhs) NOEXCEPT;
    };

    /// VTable for dynamically allocated storage.
    template<typename T>
    struct vtable_dynamic {
        static const std::type_info& type() NOEXCEPT {
            return typeid(T);
        }

        static void destroy(storage_union<>& storage) NOEXCEPT {
            //assert(reinterpret_cast<T*>(storage.dynamic));
            delete reinterpret_cast<T*>(storage.dynamic);
        }

        static void copy(const storage_union<>& src, storage_union<>& dest) {
            dest.dynamic = new T(*reinterpret_cast<const T*>(src.dynamic));
        }

        static void move(storage_union<>& src, storage_union<>& dest) NOEXCEPT {
            dest.dynamic = src.dynamic;
            src.dynamic = nullptr;
        }

        static void swap(storage_union<>& lhs, storage_union<>& rhs) NOEXCEPT {
            // just exchage the storage pointers.
            std::swap(lhs.dynamic, rhs.dynamic);
        }
    };

    /// VTable for stack allocated storage.
    template<typename T>
    struct vtable_stack {
        static const std::type_info& type() NOEXCEPT {
            return typeid(T);
        }

        static void destroy(storage_union<>& storage) NOEXCEPT {
            reinterpret_cast<T*>(&storage.stack)->~T();
        }

        static void copy(const storage_union<>& src, storage_union<>& dest) {
            new (&dest.stack) T(reinterpret_cast<const T&>(src.stack));
        }

        static void move(storage_union<>& src, storage_union<>& dest) NOEXCEPT {
            // one of the conditions for using vtable_stack is a nothrow move constructor,
            // so this move constructor will never throw a exception.
            new (&dest.stack) T(std::move(reinterpret_cast<T&>(src.stack)));
            destroy(src);
        }

        static void swap(storage_union<>& lhs, storage_union<>& rhs) NOEXCEPT {
            std::swap(reinterpret_cast<T&>(lhs.stack), reinterpret_cast<T&>(rhs.stack));
        }
    };

    /// Whether the type T must be dynamically allocated or can be stored on the stack.
    template<typename T>
    struct requires_allocation :
        std::integral_constant<bool,
        !(std::is_nothrow_move_constructible<T>::value      // N4562 §6.3/3 [any.class]
            && sizeof(T) <= sizeof(typename storage_union<>::stack_storage_t)
            && std::alignment_of<T>::value <= std::alignment_of<typename storage_union<>::stack_storage_t>::value)>
    {};

    /// Returns the pointer to the vtable of the type T.
    template<typename T>
    static vtable_type* vtable_for_type() {
        using VTableType = typename std::conditional<requires_allocation<T>::value, vtable_dynamic<T>, vtable_stack<T>>::type;
        static vtable_type table = {
            VTableType::type, VTableType::destroy,
            VTableType::copy, VTableType::move,
            VTableType::swap,
        };
        return &table;
    }

protected:
    template<typename T, size_t _StackSizeMult>
    friend const T* any_cast(const basic_any<_StackSizeMult>* operand) NOEXCEPT;
    template<typename T, size_t _StackSizeMult>
    friend T* any_cast(basic_any<_StackSizeMult>* operand) NOEXCEPT;
    template<typename T, size_t _StackSizeMult>
    friend const T* unsafe_any_cast(const basic_any<_StackSizeMult>* operand) NOEXCEPT;
    template<typename T, size_t _StackSizeMult>
    friend T* unsafe_any_cast(basic_any<_StackSizeMult>* operand) NOEXCEPT;

    /// Same effect as is_same(this->type(), t);
    bool is_typed(const std::type_info& t) const {
        return is_same(this->type(), t);
    }

    /// Checks if two type infos are the same.
    ///
    /// If ANY_IMPL_FAST_TYPE_INFO_COMPARE is defined, checks only the address of the
    /// type infos, otherwise does an actual comparision. Checking addresses is
    /// only a valid approach when there's no interaction with outside sources
    /// (other shared libraries and such).
    static bool is_same(const std::type_info& a, const std::type_info& b) {
#ifdef ANY_IMPL_FAST_TYPE_INFO_COMPARE
        return &a == &b;
#else
        return a == b;
#endif
    }

    /// Casts (with no type_info checks) the storage pointer as const T*.
    template<typename T>
    const T* cast() const NOEXCEPT {
        return requires_allocation<typename std::decay<T>::type>::value ?
            reinterpret_cast<const T*>(storage.dynamic) :
            reinterpret_cast<const T*>(&storage.stack);
    }

    /// Casts (with no type_info checks) the storage pointer as T*.
    template<typename T>
    T* cast() NOEXCEPT {
        return requires_allocation<typename std::decay<T>::type>::value ?
            reinterpret_cast<T*>(storage.dynamic) :
            reinterpret_cast<T*>(&storage.stack);
    }

private:
    storage_union<> storage; // on offset(0) so no padding for align
    vtable_type*  vtable;

    /// Chooses between stack and dynamic allocation for the type decay_t<ValueType>,
    /// assigns the correct vtable, and constructs the object on our storage.
    template<typename ValueType>
    void construct(ValueType&& value) {
        using T = typename std::decay<ValueType>::type;

        this->vtable = vtable_for_type<T>();

        if (requires_allocation<T>::value) {
            // std::cout << "NTMC:       " << std::is_nothrow_move_constructible<T>::value << std::endl;
            // std::cout << "Type size:  " << sizeof(T) << std::endl;
            // std::cout << "Stor size:  " << sizeof(typename storage_union<>::stack_storage_t) << std::endl;
            // std::cout << "Type align: " << std::alignment_of<T>::value << std::endl;
            // std::cout << "Stor align: " << std::alignment_of<typename storage_union<>::stack_storage_t>::value << std::endl;
            // std::cout << "Allocating any data to heap!" << std::endl;
            storage.dynamic = new T(std::forward<ValueType>(value));
        } else {
            // std::cout << "NTMC:       " << std::is_nothrow_move_constructible<T>::value << std::endl;
            // std::cout << "Type size: " << sizeof(T) << std::endl;
            // std::cout << "Stor size: " << sizeof(typename storage_union<>::stack_storage_t) << std::endl;
            // std::cout << "Type align: " << std::alignment_of<T>::value << std::endl;
            // std::cout << "Stor align: " << std::alignment_of<typename storage_union<>::stack_storage_t>::value << std::endl;
            // std::cout << "Allocating any data to stack!" << std::endl;
            new (&storage.stack) T(std::forward<ValueType>(value));
        }
    }
};

using any = basic_any<>;

namespace detail {
    template<typename ValueType>
    inline ValueType&& any_cast_move_if_true(typename std::remove_reference<ValueType>::type* p, std::true_type) {
        return std::move(*p);
    }

    template<typename ValueType>
    inline ValueType&& any_cast_move_if_true(typename std::remove_reference<ValueType>::type* p, std::false_type) {
        return *p;
    }
}

/// Performs *any_cast<add_const_t<remove_reference_t<ValueType>>>(&operand), or throws bad_any_cast on failure.
template<typename ValueType, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline ValueType any_cast(const basic_any<StackSizeMult>& operand) {
    auto p = any_cast<typename std::add_const<typename std::remove_reference<ValueType>::type>::type, StackSizeMult>(&operand);
    if (p == nullptr) throw bad_any_cast();
    return *p;
}

/// Performs *any_cast<remove_reference_t<ValueType>>(&operand), or throws bad_any_cast on failure.
template<typename ValueType, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline ValueType any_cast(basic_any<StackSizeMult>& operand) {
    auto p = any_cast<typename std::remove_reference<ValueType>::type, StackSizeMult>(&operand);
    if (p == nullptr) throw bad_any_cast();
    return *p;
}

///
/// If ANY_IMPL_ANYCAST_MOVEABLE is not defined, does as N4562 specifies:
///     Performs *any_cast<remove_reference_t<ValueType>>(&operand), or throws bad_any_cast on failure.
///
/// If ANY_IMPL_ANYCAST_MOVEABLE is defined, does as LWG Defect 2509 specifies:
///     If ValueType is MoveConstructible and isn't a lvalue reference, performs
///     std::move(*any_cast<remove_reference_t<ValueType>>(&operand)), otherwise
///     *any_cast<remove_reference_t<ValueType>>(&operand). Throws bad_any_cast on failure.
///
template<typename ValueType, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline ValueType any_cast(basic_any<StackSizeMult>&& operand) {
#ifdef ANY_IMPL_ANY_CAST_MOVEABLE
    // https://cplusplus.github.io/LWG/lwg-active.html#2509
    using can_move = std::integral_constant<bool,
        std::is_move_constructible<ValueType>::value
        && !std::is_lvalue_reference<ValueType>::value>;
#else
    using can_move = std::false_type;
#endif

    auto p = any_cast<typename std::remove_reference<ValueType>::type, StackSizeMult>(&operand);
    if (p == nullptr) throw bad_any_cast();
    return detail::any_cast_move_if_true<ValueType>(p, can_move());
}

/// If operand != nullptr && operand->type() == typeid(ValueType), a pointer to the object
/// contained by operand, otherwise nullptr.
template<typename T, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline const T* any_cast(const basic_any<StackSizeMult>* operand) NOEXCEPT {
    if (operand == nullptr || !operand->is_typed(typeid(T))) {
        return nullptr;
    } else {
        return operand->template cast<T>();
    }
}

/// If operand != nullptr && operand->type() == typeid(ValueType), a pointer to the object
/// contained by operand, otherwise nullptr.
template<typename T, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline T* any_cast(basic_any<StackSizeMult>* operand) NOEXCEPT {
    if (operand == nullptr || !operand->is_typed(typeid(T))) {
        return nullptr;
    } else {
        return operand->template cast<T>();
    }
}

/// Performs *any_cast<add_const_t<remove_reference_t<ValueType>>>(&operand), or throws bad_any_cast on failure.
template<typename ValueType, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline ValueType unsafe_any_cast(const basic_any<StackSizeMult>& operand) {
    auto p = unsafe_any_cast<typename std::add_const<typename std::remove_reference<ValueType>::type>::type, StackSizeMult>(&operand);
    return *p;
}

/// Performs *any_cast<remove_reference_t<ValueType>>(&operand), or throws bad_any_cast on failure.
template<typename ValueType, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline ValueType unsafe_any_cast(basic_any<StackSizeMult>& operand) {
    auto p = unsafe_any_cast<typename std::remove_reference<ValueType>::type, StackSizeMult>(&operand);
    return *p;
}

///
/// If ANY_IMPL_ANYCAST_MOVEABLE is not defined, does as N4562 specifies:
///     Performs *any_cast<remove_reference_t<ValueType>>(&operand), or throws bad_any_cast on failure.
///
/// If ANY_IMPL_ANYCAST_MOVEABLE is defined, does as LWG Defect 2509 specifies:
///     If ValueType is MoveConstructible and isn't a lvalue reference, performs
///     std::move(*any_cast<remove_reference_t<ValueType>>(&operand)), otherwise
///     *any_cast<remove_reference_t<ValueType>>(&operand). Throws bad_any_cast on failure.
///
template<typename ValueType, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline ValueType unsafe_any_cast(basic_any<StackSizeMult>&& operand) {
#ifdef ANY_IMPL_ANY_CAST_MOVEABLE
    // https://cplusplus.github.io/LWG/lwg-active.html#2509
    using can_move = std::integral_constant<bool,
        std::is_move_constructible<ValueType>::value
        && !std::is_lvalue_reference<ValueType>::value>;
#else
    using can_move = std::false_type;
#endif

    auto p = unsafe_any_cast<typename std::remove_reference<ValueType>::type, StackSizeMult>(&operand);
    return detail::any_cast_move_if_true<ValueType>(p, can_move());
}

/// If operand != nullptr && operand->type() == typeid(ValueType), a pointer to the object
/// contained by operand, otherwise nullptr.
template<typename T, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline const T* unsafe_any_cast(const basic_any<StackSizeMult>* operand) NOEXCEPT {
    if (operand == nullptr) return nullptr;

    return operand->template cast<T>();
}

/// If operand != nullptr && operand->type() == typeid(ValueType), a pointer to the object
/// contained by operand, otherwise nullptr.
template<typename T, size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
inline T* unsafe_any_cast(basic_any<StackSizeMult>* operand) NOEXCEPT {
    if (operand == nullptr) return nullptr;
        
    return operand->template cast<T>();
}

namespace std {
    template<size_t StackSizeMult = DEFAULT_ANY_STACK_SIZE_MULT>
    inline void swap(basic_any<StackSizeMult>& lhs, basic_any<StackSizeMult>& rhs) NOEXCEPT {
        lhs.swap(rhs);
    }
}

#endif //  Vorb_Any_hpp__
