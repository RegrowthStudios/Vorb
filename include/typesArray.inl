/// A common array type for unknown values
class ArrayBase {
public:
    ArrayBase(ui32 elemSize) :
        _elementSize(elemSize) {
        // Empty
    }
    ArrayBase(ui32 elemSize, void* d, size_t l) : ArrayBase(elemSize) {
        setData(d, l);
    }

    const size_t& getLength() const {
        return _length;
    }

    void setData(void* data, size_t len) {
        _length = len;
        if (_length > 0) {
            // Create a new internal array
            _dataShared.reset(new ui8[_elementSize * _length], [] (ui8* p) { delete[] p; });
            _data = _dataShared.get();

            // Copy data
            if (data) memcpy(_data, data, _elementSize * _length);
        } else {
            // Delete any old data
            _dataShared.reset();
            _data = nullptr;
        }
    }
    void setData(size_t len = 0) {
        setData(nullptr, len);
    }

    template<typename T>
    T& operator[] (size_t i) {
        return ((T*)_data)[i];
    }
    template<typename T>
    const T& operator[] (size_t i) const {
        return ((T*)_data)[i];
    }

    template<typename T>
    T& at(size_t i) {
        return ((T*)_data)[i];
    }
    template<typename T>
    const T& at(size_t i) const {
        return ((T*)_data)[i];
    }

protected:
    std::shared_ptr<ui8> _dataShared;
    void* _data = nullptr;
    ui32 _elementSize = 0;
    size_t _length = 0;
};

/// An array of known value types
template<typename T>
class Array : public ArrayBase {
public:
    Array() : ArrayBase(sizeof(T)) {
        // Empty
    }
    Array(T* d, size_t l) : ArrayBase(sizeof(T), d, l) {
        // Empty
    }

    T& operator[] (size_t i) {
        return ((T*)_data)[i];
    }
    const T& operator[] (size_t i) const {
        return ((T*)_data)[i];
    }
    T& at(size_t i) {
        return ((T*)_data)[i];
    }
    const T& at(size_t i) const {
        return ((T*)_data)[i];
    }
};
