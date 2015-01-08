///
/// RingBuffer.hpp
/// Vorb Engine
///
/// Created by Cristian Zaloj on 5 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// A ring buffer with a maximum number of elements
/// 

#pragma once

#ifndef RingBuffer_hpp__
#define RingBuffer_hpp__

namespace vorb {
    /// A constant-sized non-overwriting circular queue
    template<typename T>
    class ring_buffer {
    public:
        ring_buffer(const size_t& capacity);

        const size_t& size() const;
        size_t capacity() const;

        void resize(const size_t& s);
        void clear();

        const T& front() const;
        T& front();
        const T& back() const;
        T& back();
        const T& at() const;
        T& at();

        template<typename... Args>
        bool push(Args&&... values);
        void pop();
    private:
        ring_buffer(const ring_buffer&);
        const ring_buffer& operator=(const ring_buffer&);

        std::vector<T> m_data;
        size_t m_head = 0;
        size_t m_tail = 0;
        size_t m_elements = 0;
    };
}

#include "RingBuffer.inl"

#endif // RingBuffer_hpp__