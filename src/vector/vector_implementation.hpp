#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

class VectorInt {
private:
    int* data_;
    size_t size_;
    size_t capacity_;

    size_t calculate_new_capacity() const {
        return capacity_ == 0 ? 4 : capacity_ * 2;
    }

public:
    VectorInt() : data_(nullptr), size_(0), capacity_(0) {}
    //constructor with count and optional value
    explicit VectorInt(size_t count, int value = 0) 
        : data_(new int[count]), size_(count), capacity_(count) {
        std::fill(data_, data_ + size_, value);
    }
    //initializer list constructor
    VectorInt(std::initializer_list<int> init) 
        : data_(new int[init.size()]), size_(init.size()), capacity_(init.size()) {
        std::copy(init.begin(), init.end(), data_);
    }
    //destr
    ~VectorInt() {
        delete[] data_;
    }
    //copy constr
    VectorInt(const VectorInt& other) 
        : data_(nullptr), size_(other.size_), capacity_(other.size_) {
        if (other.size_ > 0) {
            data_ = new int[other.size_];
            std::copy(other.data_, other.data_ + other.size_, data_);
        }
    }
    
    VectorInt& operator=(const VectorInt& other) {
        if (this != &other) {
            if (capacity_ < other.size_) {
                delete[] data_;
                capacity_ = other.size_;
                data_ = new int[capacity_];
            }
            size_ = other.size_;
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }
    
    VectorInt(VectorInt&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    VectorInt& operator=(VectorInt&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    int& at(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Index not in range");
        }
        return data_[pos];
    }
    
    const int& at(size_t pos) const {
        if (pos >= size_) {
            throw std::out_of_range("Index not in range");
        }
        return data_[pos];
    }
    
    int& operator[](size_t pos) {
        return data_[pos];
    }
    
    const int& operator[](size_t pos) const {
        return data_[pos];
    }
    
    int& front() {
        return data_[0];
    }
    
    const int& front() const {
        return data_[0];
    }
    
    int& back() {
        return data_[size_ - 1];
    }
    
    const int& back() const {
        return data_[size_ - 1];
    }
    
    int* data() {
        return data_;
    }
    
    const int* data() const {
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
            int* new_data = new int[new_capacity];
            if (data_) {
                std::copy(data_, data_ + size_, new_data);
                delete[] data_;
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }
    
    void shrink_to_fit() {
        if (capacity_ > size_) {
            if (size_ == 0) {
                delete[] data_;
                data_ = nullptr;
                capacity_ = 0;
            } else {
                int* new_data = new int[size_];
                std::copy(data_, data_ + size_, new_data);
                delete[] data_;
                data_ = new_data;
                capacity_ = size_;
            }
        }
    }
    
    
    void clear() {
        size_ = 0;
    }
    
    void push_back(int value) {
        if (size_ == capacity_) {
            reserve(calculate_new_capacity());
        }
        data_[size_++] = value;
    }
    
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }
    
    void insert(size_t pos, int value) {
        if (pos > size_) {
            throw std::out_of_range("Index not in range");
        }
        
        if (size_ == capacity_) {
            size_t new_capacity = calculate_new_capacity();
            int* new_data = new int[new_capacity];
            if (data_) {
                std::copy(data_, data_ + pos, new_data);
                std::copy(data_ + pos, data_ + size_, new_data + pos + 1);
                delete[] data_;
            }
            data_ = new_data;
            capacity_ = new_capacity;
            data_[pos] = value;
            ++size_;
        } else {
            for (size_t i = size_; i > pos; --i) {
                data_[i] = data_[i - 1];
            }
            data_[pos] = value;
            ++size_;
        }
    }
    
    void erase(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Position out of range");
        }
        
        for (size_t i = pos; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        
        --size_;
    }
    
    void resize(size_t new_size, int value = 0) {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        
        if (new_size > size_) {
            std::fill(data_ + size_, data_ + new_size, value);
        }
        
        size_ = new_size;
    }
    
    void swap(VectorInt& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    
    
    int* begin() {
        return data_;
    }
    
    const int* begin() const {
        return data_;
    }
    
    int* end() {
        return data_ + size_;
    }
    
    const int* end() const {
        return data_ + size_;
    }
};

bool operator==(const VectorInt& lhs, const VectorInt& rhs) {
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

bool operator!=(const VectorInt& lhs, const VectorInt& rhs) {
    return !(lhs == rhs);
}

void swap(VectorInt& lhs, VectorInt& rhs) noexcept {
    lhs.swap(rhs);
}