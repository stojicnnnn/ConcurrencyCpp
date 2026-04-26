#ifndef VECTOR_IMPLEMENTATION_HPP
#define VECTOR_IMPLEMENTATION_HPP

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <string>
#include <memory>
#include <utility>

template<typename T>
class Vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    std::allocator<T> alloc_;

    size_t calculate_new_capacity() const {
        return capacity_ == 0 ? 4 : capacity_ * 2;
    }

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    //constructor with count and optional value
    explicit Vector(size_t count, const T& value = T()) 
        : size_(count), capacity_(count) {
        if (count > 0) {
            data_ = alloc_.allocate(count);
            for (size_t i = 0; i < size_; ++i) {
                new (data_ + i) T(value);
            }
        } else {
            data_ = nullptr;
        }
    }
    
    //initializer list constructor
    Vector(std::initializer_list<T> init) 
        : size_(init.size()), capacity_(init.size()) {
        if (size_ > 0) {
            data_ = alloc_.allocate(capacity_);
            size_t i = 0;
            for (const auto& elem : init) {
                new (data_ + i) T(elem);
                ++i;
            }
        } else {
            data_ = nullptr;
        }
    }
    
    //destr
    ~Vector() {
        if (data_) {
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~T();
            }
            alloc_.deallocate(data_, capacity_);
        }
    }
    
    //copy constr
    Vector(const Vector& other) 
        : size_(other.size_), capacity_(other.size_) {
        if (capacity_ > 0) {
            data_ = alloc_.allocate(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                new (data_ + i) T(other.data_[i]);
            }
        } else {
            data_ = nullptr;
        }
    }
    
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }
    
    Vector(Vector&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            if (data_) {
                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~T();
                }
                alloc_.deallocate(data_, capacity_);
            }
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    T& at(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Index not in range");
        }
        return data_[pos];
    }
    
    const T& at(size_t pos) const {
        if (pos >= size_) {
            throw std::out_of_range("Index not in range");
        }
        return data_[pos];
    }
    
    T& operator[](size_t pos) {
        return data_[pos];
    }
    
    const T& operator[](size_t pos) const {
        return data_[pos];
    }
    
    T& front() {
        return data_[0];
    }
    
    const T& front() const {
        return data_[0];
    }
    
    T& back() {
        return data_[size_ - 1];
    }
    
    const T& back() const {
        return data_[size_ - 1];
    }
    
    T* data() {
        return data_;
    }
    
    const T* data() const {
        return data_;
    }
    
    bool empty() const {
        return size_ == 0;
    }
    
    size_t size() const {
        return size_;
    }
    
    size_t capacity() const {
        return capacity_;
    }
    
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = alloc_.allocate(new_capacity);
            if (data_) {
                for (size_t i = 0; i < size_; ++i) {
                    new (new_data + i) T(std::move(data_[i]));
                    data_[i].~T();
                }
                alloc_.deallocate(data_, capacity_);
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }
    
    void shrink_to_fit() {
        if (capacity_ > size_) {
            if (size_ == 0) {
                if (data_) alloc_.deallocate(data_, capacity_);
                data_ = nullptr;
                capacity_ = 0;
            } else {
                T* new_data = alloc_.allocate(size_);
                for (size_t i = 0; i < size_; ++i) {
                    new (new_data + i) std::string(std::move(data_[i]));
                    data_[i].~basic_string();
                }
                alloc_.deallocate(data_, capacity_);
                data_ = new_data;
                capacity_ = size_;
            }
        }
    }
    
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
    
    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(calculate_new_capacity());
        }
        new (data_ + size_) T(value);
        size_++;
    }
    
    void push_back(T&& value) {
        if (size_ == capacity_) {
            reserve(calculate_new_capacity());
        }
        new (data_ + size_) T(std::move(value));
        size_++;
    }
    
    void pop_back() {
        if (size_ > 0) {
            --size_;
            data_[size_].~T();
        }
    }
    
    void insert(size_t pos, const T& value) {
        if (pos > size_) {
            throw std::out_of_range("Index not in range");
        }
        
        if (size_ == capacity_) {
            size_t new_capacity = calculate_new_capacity();
            T* new_data = alloc_.allocate(new_capacity);
            
            for (size_t i = 0; i < pos; ++i) {
                new (new_data + i) T(std::move(data_[i]));
                data_[i].~T();
            }
            
            new (new_data + pos) T(value);
            
            for (size_t i = pos; i < size_; ++i) {
                new (new_data + i + 1) T(std::move(data_[i]));
                data_[i].~T();
            }
            
            if (data_) {
                alloc_.deallocate(data_, capacity_);
            }
            data_ = new_data;
            capacity_ = new_capacity;
            ++size_;
        } else {
            if (size_ > pos) {
                // Construct a new element at the end from the last element
                new (data_ + size_) std::string(std::move(data_[size_ - 1]));
                // Shift existing elements
                for (size_t i = size_ - 1; i > pos; --i) {
                    data_[i] = std::move(data_[i - 1]);
                }
                data_[pos] = value;
            } else {
                new (data_ + pos) std::string(value);
            }
            ++size_;
        }
    }
    
    void erase(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Position out of range");
        }
        
        for (size_t i = pos; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        
        data_[size_ - 1].~T();
        --size_;
    }
    
    void resize(size_t new_size, const T& value = T()) {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        
        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                new (data_ + i) T(value);
            }
        } else if (new_size < size_) {
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        }
        
        size_ = new_size;
    }
    
    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    
    T* begin() {
        return data_;
    }
    
    const T* begin() const {
        return data_;
    }
    
    T* end() {
        return data_ + size_;
    }
    
    const T* end() const {
        return data_ + size_;
    }
};

template<typename T>
inline bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template<typename T>
inline bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
inline void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

using VectorString = Vector<std::string>;

#endif // VECTOR_IMPLEMENTATION_HPP