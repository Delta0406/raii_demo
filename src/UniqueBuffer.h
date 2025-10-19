//
// Created by wnc on 2025/10/19.
//

#ifndef RAII_DEMO_UNIQUEBUFFER_H
#define RAII_DEMO_UNIQUEBUFFER_H

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
class UniqueBuffer {
public:
    // 构造：分配 n 个元素
    explicit UniqueBuffer(size_t size)
        : size_(size), data_(allocate(size_), FreeDeleter<T>{}) {}

    // 构造：使用外部已有指针，可选择是否接管
    UniqueBuffer(T* externalPtr, size_t size, bool takeOwnership = true)
        : size_(size) {
        if (takeOwnership)
            data_ = std::unique_ptr<T, FreeDeleter<T>>(externalPtr);
        else
            data_ = std::unique_ptr<T, FreeDeleter<T>>(externalPtr, [](T*){}); // 不释放
    }

    // 禁止拷贝（独占语义）
    UniqueBuffer(const UniqueBuffer&) = delete;
    UniqueBuffer& operator=(const UniqueBuffer&) = delete;

    // 支持移动
    UniqueBuffer(UniqueBuffer&&) noexcept = default;
    UniqueBuffer& operator=(UniqueBuffer&&) noexcept = default;

    // 访问数据
    T* data() noexcept { return data_.get(); }
    const T* data() const noexcept { return data_.get(); }
    size_t size() const noexcept { return size_; }

    // 数组访问
    T& operator[](size_t idx) { return data_.get()[idx]; }
    const T& operator[](size_t idx) const { return data_.get()[idx]; }

    // 填充数据
    void fill(const T& value) {
        if constexpr (std::is_trivial<T>::value) {
            std::memset(data(), static_cast<int>(value), size_ * sizeof(T));
        } else {
            std::fill(data(), data() + size_, value);
        }
    }

private:
    size_t size_;
    std::unique_ptr<T, FreeDeleter<T>> data_;

    static T* allocate(size_t n) {
        void* ptr = std::malloc(n * sizeof(T));
        if (!ptr) throw std::bad_alloc();
        return static_cast<T*>(ptr);
    }
};

#endif //RAII_DEMO_UNIQUEBUFFER_H