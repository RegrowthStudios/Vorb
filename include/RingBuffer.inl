namespace impl {
    template<typename T, typename Arg1, typename... Args>
    inline void moveAll(std::vector<T>& data, const size_t& i, Arg1&& v1, Args&&... values) {
        data[i % data.size()] = std::move<Arg1>(v1);
        moveAll(data, i + 1, values...);
    }
    template<typename T, typename Arg1>
    inline void moveAll(std::vector<T>& data, const size_t& i, Arg1&& v1) {
        data[i % data.size()] = std::move<Arg1>(v1);
    }

    template<typename Arg1, typename... Args>
    inline size_t numArgs(Arg1& v1, Args&... values) {
        return 1 + numArgs(values...);
    }
    inline size_t numArgs() {
        return 0;
    }
}

template<typename T>
vorb::ring_buffer<T>::ring_buffer(const size_t& capacity) : 
    m_data(capacity) {
    if (capacity == 0) throw std::exception("Capacity must be non-zero");
}

template<typename T>
const size_t& vorb::ring_buffer<T>::size() const {
    return m_elements;
}
template<typename T>
size_t vorb::ring_buffer<T>::capacity() const {
    return m_data.capacity();
}

template<typename T>
void vorb::ring_buffer<T>::resize(const size_t& s) {
    if (s == 0) throw std::exception("Capacity must be non-zero");

    // Don't need to resize
    if (s == m_data.size()) return;

    // If size if smaller than number of elements, faux-pop elements
    if (s < m_elements) {
        m_tail += m_elements - s;
        m_tail %= m_data.capacity();
        m_elements = s;
    }

    // Allocate new vector
    std::vector<T> copy(s);
    size_t i = 0;

    // Check for a split buffer
    if (m_tail > m_head || m_elements > 0) {
        // Pop from tail to end
        while (m_tail < m_data.capacity()) {
            copy[i++] = m_data[m_tail++];
        }
        m_tail = 0;
    }

    // Pop from tail to head
    while (m_tail < m_head) {
        copy[i++] = m_data[m_tail++];
    }

    // Swap contents
    m_data.swap(copy);

    // Update new head and tail
    m_head = m_elements;
    m_tail = 0;
}
template<typename T>
void vorb::ring_buffer<T>::clear() {
    // Faux-clear
    m_head = 0;
    m_tail = 0;
    m_elements = 0;
}

template<typename T>
const T& vorb::ring_buffer<T>::front() const {
    return m_data[m_tail];
}
template<typename T>
T& vorb::ring_buffer<T>::front() {
    return m_data[m_tail];
}
template<typename T>
const T& vorb::ring_buffer<T>::back() const {
    size_t i = m_head + (m_data.capacity() - 1);
    return m_data[i % m_data.capacity()];
}
template<typename T>
T& vorb::ring_buffer<T>::back() {
    size_t i = m_head + (m_data.capacity() - 1);
    return m_data[i % m_data.capacity()];
}
template<typename T>
const T& vorb::ring_buffer<T>::at(const size_t& i) const {
    size_t ind = m_tail + i;
    return m_data[ind % m_data.capacity()];
}
template<typename T>
T& vorb::ring_buffer<T>::at(const size_t& i) {
    size_t ind = m_tail + i;
    return m_data[ind % m_data.capacity()];
}

template<typename T>
void vorb::ring_buffer<T>::pop() {
    if (m_elements == 0) return;
    m_elements--;

    m_tail++;
    m_tail %= m_data.capacity();
}
template<typename T>
template<typename... Args>
bool vorb::ring_buffer<T>::push(Args&&... values) {
    size_t numElements = impl::numArgs(values...);
    if (m_elements + numElements > m_data.capacity()) return false;
    m_elements += numElements;

    // Add data at head
    impl::moveAll(m_data, m_head, values...);

    // Move head
    m_head += numElements;
    m_head %= m_data.capacity();
    return true;
}

