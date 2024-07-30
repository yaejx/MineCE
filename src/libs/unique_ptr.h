#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

// Custom implementation of remove_extent
template<typename T>
struct remove_extent { typedef T type; };

template<typename T>
struct remove_extent<T[]> { typedef T type; };

template<typename T, size_t N>
struct remove_extent<T[N]> { typedef T type; };

template<typename T>
class unique_ptr {
private:
    T* ptr;

public:
    explicit unique_ptr(T* p = nullptr) : ptr(p) {}
    
    ~unique_ptr() {
        delete ptr;
    }

    unique_ptr(unique_ptr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    explicit operator bool() const { return ptr != nullptr; }
};

// Specialization for array types
template<typename T>
class unique_ptr<T[]> {
private:
    T* ptr;

public:
    explicit unique_ptr(T* p = nullptr) : ptr(p) {}
    
    ~unique_ptr() {
        delete[] ptr;
    }

    unique_ptr(unique_ptr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            delete[] ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    T& operator[](size_t i) const { return ptr[i]; }
    T* get() const { return ptr; }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        delete[] ptr;
        ptr = p;
    }

    explicit operator bool() const { return ptr != nullptr; }
};

// Non-array version
template<typename T, typename... Args>
typename std::enable_if<!std::is_array<T>::value, unique_ptr<T>>::type
make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// Array version
template<typename T>
typename std::enable_if<std::is_array<T>::value, unique_ptr<T>>::type
make_unique(size_t size) {
    typedef typename remove_extent<T>::type U;
    return unique_ptr<T>(new U[size]());
}

#endif // UNIQUE_PTR_H
