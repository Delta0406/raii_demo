//
// Created by wnc on 2025/10/19.
//

#ifndef RAII_DEMO_SHAREDBUFFER_H
#define RAII_DEMO_SHAREDBUFFER_H

#include <memory>
#include <cstdlib>
#include <cstring>
#include <algorithm>

template<typename T>
struct FreeDeleter {
    void operator()(T* p) const noexcept {
        if (p) std::free(p);
    }
};

template<typename T>
class SharedBuffer {
public:
    explicit SharedBuffer(size_t size)
        : size_(size), data_(allocate(size_), FreeDeleter<T>{}) {}

    SharedBuffer(T* externalPtr, size_t size, bool takeOwnership = true)
        : size_(size) {
        if (takeOwnership)
            data_ = std::shared_ptr<T>(externalPtr, FreeDeleter<T>{});
        else
            data_ = std::shared_ptr<T>(externalPtr, [](T*){});
    }

    SharedBuffer(const SharedBuffer&) = default;
    SharedBuffer& operator=(const SharedBuffer&) = default;
    SharedBuffer(SharedBuffer&&) noexcept = default;
    SharedBuffer& operator=(SharedBuffer&&) noexcept = default;

    T* data() noexcept { return data_.get(); }
    const T* data() const noexcept { return data_.get(); }
    size_t size() const noexcept { return size_; }

    void fill(const T& value) {
        if constexpr (std::is_trivial<T>::value) {
            std::memset(data(), static_cast<int>(value), size_ * sizeof(T));
        } else {
            std::fill(data(), data() + size_, value);
        }
    }

private:
    size_t size_;
    std::shared_ptr<T> data_;

    static T* allocate(size_t n) {
        void* ptr = std::malloc(n * sizeof(T));
        if (!ptr) throw std::bad_alloc();
        return static_cast<T*>(ptr);
    }
};

#endif //RAII_DEMO_SHAREDBUFFER_H