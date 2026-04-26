#ifndef VECTOR_IMPLEMENTATION_HPP
#define VECTOR_IMPLEMENTATION_HPP

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <string>
#include <memory>
#include <utility>

class VectorString {
private:
    std::string* data_;
    size_t size_;
    size_t capacity_;
    std::allocator<std::string> alloc_;

    size_t calculate_new_capacity() const {
        return capacity_ == 0 ? 4 : capacity_ * 2;
    }

public:
    VectorString() : data_(nullptr), size_(0), capacity_(0) {}
    
    //constructor with count and optional value
    explicit VectorString(size_t count, const std::string& value = "") 
        : size_(count), capacity_(count) {
        if (count > 0) {
            data_ = alloc_.allocate(count);
            for (size_t i = 0; i < size_; ++i) {
                new (data_ + i) std::string(value);
            }
        } else {
            data_ = nullptr;
        }
    }
    
    //initializer list constructor
    VectorString(std::initializer_list<std::string> init) 
        : size_(init.size()), capacity_(init.size()) {
        if (size_ > 0) {
            data_ = alloc_.allocate(capacity_);
            size_t i = 0;
            for (const auto& elem : init) {
                new (data_ + i) std::string(elem);
                ++i;
            }
        } else {
            data_ = nullptr;
        }
    }
    
    //destr
    ~VectorString() {
        if (data_) {
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~basic_string();
            }
            alloc_.deallocate(data_, capacity_);
        }
    }
    
    //copy constr
    VectorString(const VectorString& other) 
        : size_(other.size_), capacity_(other.size_) {
        if (capacity_ > 0) {
            data_ = alloc_.allocate(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                new (data_ + i) std::string(other.data_[i]);
            }
        } else {
            data_ = nullptr;
        }
    }
    
    VectorString& operator=(const VectorString& other) {
        if (this != &other) {
            VectorString temp(other);
            swap(temp);
        }
        return *this;
    }
    
    VectorString(VectorString&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    VectorString& operator=(VectorString&& other) noexcept {
        if (this != &other) {
            if (data_) {
                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~basic_string();
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
    
    std::string& at(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Index not in range");
        }
        return data_[pos];
    }
    
    const std::string& at(size_t pos) const {
        if (pos >= size_) {
            throw std::out_of_range("Index not in range");
        }
        return data_[pos];
    }
    
    std::string& operator[](size_t pos) {
        return data_[pos];
    }
    
    const std::string& operator[](size_t pos) const {
        return data_[pos];
    }
    
    std::string& front() {
        return data_[0];
    }
    
    const std::string& front() const {
        return data_[0];
    }
    
    std::string& back() {
        return data_[size_ - 1];
    }
    
    const std::string& back() const {
        return data_[size_ - 1];
    }
    
    std::string* data() {
        return data_;
    }
    
    const std::string* data() const {
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
            std::string* new_data = alloc_.allocate(new_capacity);
            if (data_) {
                for (size_t i = 0; i < size_; ++i) {
                    new (new_data + i) std::string(std::move(data_[i]));
                    data_[i].~basic_string();
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
                std::string* new_data = alloc_.allocate(size_);
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
            data_[i].~basic_string();
        }
        size_ = 0;
    }
    
    void push_back(const std::string& value) {
        if (size_ == capacity_) {
            reserve(calculate_new_capacity());
        }
        new (data_ + size_) std::string(value);
        size_++;
    }
    
    void push_back(std::string&& value) {
        if (size_ == capacity_) {
            reserve(calculate_new_capacity());
        }
        new (data_ + size_) std::string(std::move(value));
        size_++;
    }
    
    void pop_back() {
        if (size_ > 0) {
            --size_;
            data_[size_].~basic_string();
        }
    }
    
    void insert(size_t pos, const std::string& value) {
        if (pos > size_) {
            throw std::out_of_range("Index not in range");
        }
        
        if (size_ == capacity_) {
            size_t new_capacity = calculate_new_capacity();
            std::string* new_data = alloc_.allocate(new_capacity);
            
            for (size_t i = 0; i < pos; ++i) {
                new (new_data + i) std::string(std::move(data_[i]));
                data_[i].~basic_string();
            }
            
            new (new_data + pos) std::string(value);
            
            for (size_t i = pos; i < size_; ++i) {
                new (new_data + i + 1) std::string(std::move(data_[i]));
                data_[i].~basic_string();
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
        
        data_[size_ - 1].~basic_string();
        --size_;
    }
    
    void resize(size_t new_size, const std::string& value = "") {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        
        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                new (data_ + i) std::string(value);
            }
        } else if (new_size < size_) {
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~basic_string();
            }
        }
        
        size_ = new_size;
    }
    
    void swap(VectorString& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    
    std::string* begin() {
        return data_;
    }
    
    const std::string* begin() const {
        return data_;
    }
    
    std::string* end() {
        return data_ + size_;
    }
    
    const std::string* end() const {
        return data_ + size_;
    }
};

inline bool operator==(const VectorString& lhs, const VectorString& rhs) {
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

inline bool operator!=(const VectorString& lhs, const VectorString& rhs) {
    return !(lhs == rhs);
}

inline void swap(VectorString& lhs, VectorString& rhs) noexcept {
    lhs.swap(rhs);
}

#endif // VECTOR_IMPLEMENTATION_HPP