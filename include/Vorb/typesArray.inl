/*! \file typesArray.inl
 * @brief A definition for a very simple array type.
 */

/*! @brief The common root for an array type.
 *
 * This common root functions in a similar manner as void* for pointers.
 * This array contains element size metadata in order to store accurate
 * length information. It is recommended that only POD data types are
 * used since no constructors or destructors are called.
 */
 #include <cstring>

class ArrayBase {
    template <typename T> friend class Array;

public:
    /*! @brief Default array constructor with no data
     */
    ArrayBase():
        m_data(nullptr),
        m_elementSize(0),
        m_length(0)
    {}
    /*! @brief Construct an array with element information.
     *
     * @param elemSize: Size of an element in bytes
     */
    ArrayBase(size_t elemSize) :
        m_data(nullptr),
        m_elementSize(elemSize),
        m_length(0)
    {
        // Empty
    }
    /*! @brief Construct an array with element information and give it existing data.
     *
     * The array copies over all the data found within the data block.
     * It becomes the first owner of the data.
     *
     * @param elemSize: Size of an element in bytes
     * @param d: Pointer to a block of element data (garbage if nullptr)
     * @param l: Number of elements found in the data
     */
    ArrayBase(size_t elemSize, void* d, size_t l) : ArrayBase(elemSize) {
        setData(d, l);
    }

    /*! @brief Obtain the number of elements in the array.
     *
     * @return Number of elements in the array (0 for none)
     */
    const size_t& size() const {
        return m_length;
    }

    /*! @brief Make this array to hold a new set of data
     *
     * The array copies over all the data found within the data block.
     * It becomes the first owner of the data.
     *
     * @param data: Pointer to a block of element data (garbage if nullptr)
     * @param len: Number of elements found in the data
     */
    void setData(void* data, size_t len) {
        m_length = len;
        if (m_length > 0) {
            // Create a new internal array
            m_sharedData.reset(new ui8[m_elementSize * m_length], [] (ui8* p) { delete[] p; });
            m_data = m_sharedData.get();

            // Copy data
            if (data) memcpy(m_data, data, m_elementSize * m_length);
        } else {
            // Delete any old data
            m_sharedData.reset();
            m_data = nullptr;
        }
    }
    /*! @brief Make this array to hold a new set of garbage data
     *
     * The array creates a new block of garbage data.
     * It becomes the first owner of the data.
     *
     * @param len: Number of elements in this array
     */
    void setData(size_t len = 0) {
        setData(nullptr, len);
    }

    /*! @brief Make this array the owner of a set of data
     *
     * @pre: All previous ownership of data is now invalidated
     * @tparam T: Array object type
     * @param data: Pointer to a block of element data
     * @param len: Number of elements found in the data
     */
    void ownData(CALLEE_DELETE void* data, size_t len, void(*fDealloc)(void*)) {
        m_length = len;
        m_sharedData.reset((ui8*)data, [=] (ui8* p) { fDealloc(p); });
        m_data = m_sharedData.get();
    }

    /*! @brief Obtain a reference to an element in the array.
     *
     * There is no bounds checking performed on the input. If
     * the array is destroyed (all sharing Arrays), any pointers referring to the element
     * are invalid.
     *
     * @tparam T: Type of element to be obtained
     * @param i: Index of the element
     * @return Element reference
     */
    template<typename T>
    T& operator[] (const size_t& i) {
        return ((T*)m_data)[i];
    }
    /*! @brief Obtain a reference to an element in the array.
     *
     * There is no bounds checking performed on the input. If
     * the array is destroyed (all sharing Arrays), any pointers referring to the element
     * are invalid.
     *
     * @tparam T: Type of element to be obtained
     * @param i: Index of the element
     * @return Element reference
     */
    template<typename T>
    const T& operator[] (const size_t& i) const {
        return ((T*)m_data)[i];
    }

    /*! @brief Obtain a reference to an element in the array.
     *
     * There is no bounds checking performed on the input. If
     * the array is destroyed (all sharing Arrays), any pointers referring to the element
     * are invalid.
     *
     * @tparam T: Type of element to be obtained
     * @param i: Index of the element
     * @return Element reference
     */
    template<typename T>
    T& at(const size_t& i) {
        return ((T*)m_data)[i];
    }
    /*! @brief Obtain a reference to an element in the array.
     *
     * There is no bounds checking performed on the input. If
     * the array is destroyed (all sharing Arrays), any pointers referring to the element
     * are invalid.
     *
     * @tparam T: Type of element to be obtained
     * @param i: Index of the element
     * @return Element reference
     */
    template<typename T>
    const T& at(const size_t& i) const {
        return ((T*)m_data)[i];
    }

protected:
    std::shared_ptr<ui8> m_sharedData; ///< Shared data pointer to allow for automatic memory management
    void* m_data; ///< Cached pointer from the shared data
    size_t m_elementSize; ///< The size of the elements in bytes
    size_t m_length; ///< The length of the array in elements
};

/*! @brief An array filled with known element types.
 *
 * @tparam T: The type of element found in the array
 */
template<typename T>
class Array : public ArrayBase {

public:
    /*! @brief Empty array constructor.
     *
     * This constructor correctly intializes element size.
     */
    Array() : ArrayBase(sizeof(T)) {
        // Empty
    }
    /*! @brief Construct an array with from existing data.
     *
     * The array copies over all the data found within the data block.
     * It becomes the first owner of the data.
     *
     * @param d: Pointer to a block of data (garbage if nullptr)
     * @param l: Number of elements found in the data
     */
    Array(T* d, size_t l) : ArrayBase(sizeof(T), d, l) {
        // Empty
    }
    /*! @brief Make this array a shadow copy of another array.
     *
     * This array co-owns the data stored in the input array at the
     * time of the copy.
     *
     * @param arr: Array reference for co-ownership
     */
    Array(const ArrayBase& arr) {
        // Simple copy
        m_sharedData = arr.m_sharedData;
        m_data = arr.m_data;
        m_elementSize = arr.m_elementSize;
        m_length = arr.m_length;
    }

    /*! @brief Obtain a reference to an element in the array.
     *
     * There is no bounds checking performed on the input. If
     * the array is destroyed (all sharing Arrays), any pointers referring to the element
     * are invalid.
     *
     * @param i: Index of the element
     * @return Element reference
     */
    T& operator[] (const size_t& i) {
        return ((T*)m_data)[i];
    }
    /*! @brief Obtain a reference to an element in the array.
     *
     * There is no bounds checking performed on the input. If
     * the array is destroyed (all sharing Arrays), any pointers referring to the element
     * are invalid.
     *
     * @param i: Index of the element
     * @return Element reference
     */
    const T& operator[] (const size_t& i) const {
        return ((T*)m_data)[i];
    }
    /*! @brief Obtain a reference to an element in the array.
     *
     * There is no bounds checking performed on the input. If
     * the array is destroyed (all sharing Arrays), any pointers referring to the element
     * are invalid.
     *
     * @param i: Index of the element
     * @return Element reference
     */
    T& at(const size_t& i) {
        return ((T*)m_data)[i];
    }
    /*! @brief Obtain a reference to an element in the array.
     *
     * There is no bounds checking performed on the input. If
     * the array is destroyed (all sharing Arrays), any pointers referring to the element
     * are invalid.
     *
     * @param i: Index of the element
     * @return Element reference
     */
    const T& at(const size_t& i) const {
        return ((T*)m_data)[i];
    }


    /*!
     * \brief Enables iteration over the contents of the array.
     *
     * \return A pointer to the first element stored.
     */
    T* begin() {
        return m_length > 0 ? &((T*)m_data)[0] : nullptr;
    }
    /*!
     * \brief Enables iteration over the contents of the array.
     *
     * \return A pointer to the first element stored.
     */
    T* end() {
        return m_length > 0 ? &((T*)m_data)[m_length - 1] : nullptr;
    }
    /*!
     * \brief Enables iteration over the contents of the array.
     *
     * \return A pointer to the first element stored.
     */
    const T* begin() const {
        return m_length > 0 ? &((T*)m_data)[0] : nullptr;
    }
    /*!
     * \brief Enables iteration over the contents of the array.
     *
     * \return A pointer to the first element stored.
     */
    const T* end() const {
        return m_length > 0 ? &((T*)m_data)[m_length - 1] : nullptr;
    }
};

/*! \example "Vorb Array Usage"
 *
 * Due to the lack of an array type in C++, an Array(Base) class has been implemented for simple use cases.
 * \include VorbArray.cpp
 *
 * By far, the most useful case of an array is the ability to use it with Keg (it was developed for this
 * purpose). Keg's parseable array value uses Array(Base) to house its data.
 * \include VorbArrayKeg.cpp
 */
