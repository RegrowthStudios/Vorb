///
/// PtrRecycler.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 8 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
///
///

#pragma once

#ifndef PtrRecycler_h__
#define PtrRecycler_h__

/*! @brief Creates and caches larges numbers of pointers.
 * 
 * Objects are created as unique pointers and one at a time. The
 * recycler will free all objects when it is destroyed.
 * 
 * @tparam T: Data type pointers
 */
template<typename T>
class PtrRecycler {
public:
    PtrRecycler() {
        // Empty
    }
    /*! @brief Frees all constructed objects.
     * 
     * The destructor calls freeAll().
     */
    ~PtrRecycler() {
        freeAll();
    }


    /*! @brief Obtain a new object.
     * 
     * Objects created using this method should be recycled in
     * order to preserve memory.
     * 
     * @tparam Args: Types of constructor arguments
     * @param args: Argument values for object constructor
     * @return A pointer to an object
     */
    template<typename... Args>
    CALLEE_DELETE T* create(Args... args) {
        T* data;
        if (m_recycled.size() > 0) {
            // Get a recycled data
            data = m_recycled[m_recycled.size() - 1];

            // Update lists
            m_recycled.pop_back();
            *getCounter(data) = 0;
        } else {
            // Create a new data segment
            PtrBind* bind = (PtrBind*)malloc(sizeof(PtrBind));
            bind->recycleCheck = 0;
            data = &bind->data;

            // Add The Data Checks
            m_allocated.push_back(bind);
        }

        // Call the constructor
        new (data)T(args...);
        return data;
    }

    /*! @brief Recycle the memory for later use.
     * 
     * The object's destructor will be called, and it
     * should not be used after this point.
     * 
     * @param data: Object to be destroyed
     */
    void recycle(T* data) {
        // Make sure it hasn't already been recycled
        i32* recycleCheck = getCounter(data);
        if (*recycleCheck == 0) {
            m_recycled.push_back(data);
            *recycleCheck = 1;

            // Call the destructor
            data->~T();
        }
    }

    /*! @brief 
     * 
     * 
     * 
     * @return 
     */
    void freeAll() {
        if (m_allocated.size() > 0) {
            // Free all allocated memory
            for (size_t i = m_allocated.size(); i > 0;) {
                i--;

                // Call the destructor if necessary
                if (m_allocated[i]->recycleCheck == 0) {
                    m_allocated[i]->data.~T();
                }

                // Free the data
                free(m_allocated[i]);
            }

            // Empty out the lists
            std::vector<PtrBind*>().swap(m_allocated);
            std::vector<T*>().swap(m_recycled);
        }
    }
private:
    // The recycler may not be copied to avoid ownership issues
    VORB_NON_COPYABLE(PtrRecycler);

    struct PtrBind {
    public:
        T data; ///< Object value
        i32 recycleCheck = 0; ///< Counter that tracks recycling calls
    };

    i32* getCounter(T* data) {
        auto ptr = (typename PtrRecycler<T>::PtrBind*)data;
        return &(ptr->recycleCheck);
    }

    std::vector<PtrBind*> m_allocated;
    std::vector<T*> m_recycled;
};

namespace {
    template<typename T>
    class Wrapper {
    public:
        Wrapper(T v) :
            value(v) {
            // Empty
        }
        operator T() const {
            return value;
        }
        T value;
    };
}

#define VORB_PTR_RECYCLER_WRAP_PRIMITIVE(TYPE) \
template<> class PtrRecycler<TYPE> : private PtrRecycler<Wrapper<TYPE>> { \
public: \
    template<typename... Args> \
    TYPE* create(Args... args) { \
        return (TYPE*)PtrRecycler<Wrapper<TYPE>>::create(args...); \
    } \
    void recycle(TYPE* data) { \
        PtrRecycler<Wrapper<TYPE>>::recycle((Wrapper<TYPE>*)data); \
    } \
    void freeAll() { \
        PtrRecycler<Wrapper<TYPE>>::freeAll(); \
    } \
}

#endif // PtrRecycler_h__