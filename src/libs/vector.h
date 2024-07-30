#ifndef VECTOR_H
#define VECTOR_H

#include <new>
#include <utility>

// Define iterator tags as not available in std::
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <typename T>
class vector {
private:
    T *data;
    size_t sz;
    size_t cap;

    void resize(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < sz; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        cap = new_cap;
    }

public:
    vector() : data(nullptr), sz(0), cap(0) {}
    
    vector(size_t size, const T &value = T()) : sz(size), cap(size) {
        data = new T[cap];
        for (size_t i = 0; i < sz; ++i) {
            data[i] = value;
        }
    }
    
    ~vector() {
        delete[] data;
    }
    
    vector(const vector &other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (size_t i = 0; i < sz; ++i) {
            data[i] = other.data[i];
        }
    }
    
    vector &operator=(const vector &other) {
        if (this != &other) {
            delete[] data;
            sz = other.sz;
            cap = other.cap;
            data = new T[cap];
            for (size_t i = 0; i < sz; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    vector(vector &&other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }
    
    vector &operator=(vector &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    size_t size() const { return sz; }
    size_t capacity() const { return cap; }
    
    void push_back(const T &value) {
        if (sz == cap) {
            resize(cap == 0 ? 1 : 2 * cap);
        }
        data[sz++] = T(value);  // Create a new object and move it
    }
    
    void push_back(T &&value) {
        if (sz == cap) {
            resize(cap == 0 ? 1 : 2 * cap);
        }
        data[sz++] = std::move(value);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (sz == cap) {
            resize(cap == 0 ? 1 : 2 * cap);
        }
        new (data + sz) T(std::forward<Args>(args)...);
        ++sz;
    }
    
    void pop_back() {
        if (sz > 0) {
            --sz;
        }
    }
    
    T &operator[](size_t index) {
        return data[index];
    }
    
    const T &operator[](size_t index) const {
        return data[index];
    }
    
    void clear() { sz = 0; }
    
    void reserve(size_t new_cap) {
        if (new_cap > cap) {
            resize(new_cap);
        }
    }
    
    void shrink_to_fit() {
        if (sz < cap) {
            resize(sz);
        }
    }
    
    void insert(size_t index, const T &value) {
        if (sz == cap) {
            resize(cap == 0 ? 1 : 2 * cap);
        }
        for (size_t i = sz; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = value;
        ++sz;
    }
    
    void erase(size_t index) {
        for (size_t i = index; i < sz - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --sz;
    }

    // Iterator classes
    class iterator {
    private:
        T *ptr;

    public:
        using iterator_category = random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        iterator(T *ptr) : ptr(ptr) {}

        reference operator*() const { return *ptr; }
        pointer operator->() { return ptr; }

        iterator &operator++() {
            ++ptr;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        iterator &operator--() {
            --ptr;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            --ptr;
            return tmp;
        }

        iterator operator+(difference_type offset) const { return iterator(ptr + offset); }
        iterator operator-(difference_type offset) const { return iterator(ptr - offset); }

        difference_type operator-(const iterator &other) const { return ptr - other.ptr; }

        bool operator==(const iterator &other) const { return ptr == other.ptr; }
        bool operator!=(const iterator &other) const { return ptr != other.ptr; }
        bool operator<(const iterator &other) const { return ptr < other.ptr; }
        bool operator>(const iterator &other) const { return ptr > other.ptr; }
        bool operator<=(const iterator &other) const { return ptr <= other.ptr; }
        bool operator>=(const iterator &other) const { return ptr >= other.ptr; }
    };

    class const_iterator {
    private:
        const T *ptr;

    public:
        using iterator_category = random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const T *;
        using reference = const T &;

        const_iterator(const T *ptr) : ptr(ptr) {}

        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }

        const_iterator &operator++() {
            ++ptr;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        const_iterator &operator--() {
            --ptr;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --ptr;
            return tmp;
        }

        const_iterator operator+(difference_type offset) const { return const_iterator(ptr + offset); }
        const_iterator operator-(difference_type offset) const { return const_iterator(ptr - offset); }

        difference_type operator-(const const_iterator &other) const { return ptr - other.ptr; }

        bool operator==(const const_iterator &other) const { return ptr == other.ptr; }
        bool operator!=(const const_iterator &other) const { return ptr != other.ptr; }
        bool operator<(const const_iterator &other) const { return ptr < other.ptr; }
        bool operator>(const const_iterator &other) const { return ptr > other.ptr; }
        bool operator<=(const const_iterator &other) const { return ptr <= other.ptr; }
        bool operator>=(const const_iterator &other) const { return ptr >= other.ptr; }
    };

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + sz); }
    const_iterator begin() const { return const_iterator(data); }
    const_iterator end() const { return const_iterator(data + sz); }
    const_iterator cbegin() const { return const_iterator(data); }
    const_iterator cend() const { return const_iterator(data + sz); }
};

#endif // VECTOR_H
