#ifndef _XENODON_UTILITY_SPAN_H
#define _XENODON_UTILITY_SPAN_H

#include <cstddef>
#include <array>
#include <vector>
#include <initializer_list>
#include <vulkan/vulkan.hpp>

template <typename T>
class Span: std::random_access_iterator_tag {
    size_t count;
    T* ptr;

public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    constexpr Span(std::nullptr_t):
        count(0),
        ptr(nullptr) {
    }

    constexpr Span(T& ptr):
        count(1),
        ptr(&ptr) {
    }

    constexpr Span(uint32_t count, T* ptr):
        count(count),
        ptr(ptr) {
    }

    template <size_t N>
    constexpr Span(std::array<typename std::remove_const<T>::type, N>& data):
        count(N),
        ptr(data.data()) {
    }

    template <size_t N>
    constexpr Span(const std::array<typename std::remove_const<T>::type, N>& data):
        count(N),
        ptr(data.data()) {
    }

    Span(vk::ArrayProxy<T> proxy):
        count(proxy.size()),
        ptr(proxy.data()) {
    }

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    Span(std::vector<typename std::remove_const<T>::type, Allocator> & data):
        count(data.size()),
        ptr(data.data()) {
    }

    template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
    Span(const std::vector<typename std::remove_const<T>::type, Allocator>& data):
        count(data.size()),
        ptr(data.data()) {
    }

    constexpr Span(std::initializer_list<T> const& data):
        count(data.end() - data.begin()),
        ptr(data.begin()) {
    }

    constexpr T* begin() {
        return ptr;
    }

    constexpr const T* begin() const {
        return ptr;
    }

    constexpr T* end() {
        return ptr + count;
    }

    constexpr const T* end() const {
        return ptr + count;
    }

    constexpr T& front() {
      return *this->ptr;
    }

    constexpr const T & front() const {
      return *this->ptr;
    }

    constexpr T& back() {
        return *(this->ptr + this->count - 1);
    }

    constexpr const T& back() const {
        return *(this->ptr + this->count - 1);
    }

    constexpr bool empty() const {
        return (this->count == 0);
    }

    constexpr uint32_t size() const {
        return this->count;
    }

    constexpr T* data() {
        return this->ptr;
    } 

    constexpr const T* data() const {
        return this->ptr;
    }

    constexpr T& operator[](size_t i) {
        return this->ptr[i];
    }

    constexpr const T& operator[](size_t i) const {
        return this->ptr[i];
    }
};

#endif