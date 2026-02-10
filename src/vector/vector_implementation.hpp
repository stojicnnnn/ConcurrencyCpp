#include <stdexcept>
#include <algorithm>
#include <initializer_list>

class VectorInt {
private:
    int* data;
    std::size_t size_;
    std::size_t capacity_;

public:
    VectorInt() : data(nullptr), size_(0), capacity_(0) {}
    //constructor with count and optional value
    explicit VectorInt(std::size_t count, int value = 0) 
        : data(new int[count]), size_(count), capacity_(count) {
        std::fill(data, data + size_, value);
    }
    //initializer list constructor
    VectorInt(std::initializer_list<int> init) 
        : data(new int[init.size()]), size_(init.size()), capacity_(init.size()) {
        std::copy(init.begin(), init.end(), data);
    }
    //destr
    ~VectorInt() {
        delete[] data;
    }
    //copy constr
    VectorInt(const VectorInt& other) 
        : data(nullptr), size_(other.size_), capacity_(other.capacity_) {
        if (other.capacity_ > 0) {
            data = new int[other.capacity_];
            std::copy(other.data, other.data + other.size_, data);
        }
    }
    
    VectorInt& operator=(const VectorInt& other) {
        if (this != &other) {
            // copy and swap??
            VectorInt temp(other);
            swap(temp);
        }
        return *this;
    }
    
    VectorInt(VectorInt&& other) noexcept 
        : data(other.data), size_(other.size_), capacity_(other.capacity_) {
        other.data = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    VectorInt& operator=(VectorInt&& other) noexcept {
        if (this != &other) {
            delete[] data;
            
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    int& at(std::size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Index not in raneg");
        }
        return data[pos];
    }
    
    const int& at(std::size_t pos) const {
        if (pos >= size_) {
            throw std::out_of_range("INdex not in raneg");
        }
        return data[pos];
    }
    
    int& operator[](std::size_t pos) {
        return data[pos];
    }
    
    const int& operator[](std::size_t pos) const {
        return data[pos];
    }
    
    int& front() {
        return data[0];
    }
    
    const int& front() const {
        return data[0];
    }
    
    int& back() {
        return data[size_ - 1];
    }
    
    const int& back() const {
        return data[size_ - 1];
    }
    
    int* get_data() {
        return data;
    }
    
    const int* get_data() const {
        return data;
    }
    
    
    bool empty() const {
        return size_ == 0;
    }
    
    std::size_t size() const {
        return size_;
    }
    
    std::size_t capacity() const {
        return capacity_;
    }
    
    void reserve(std::size_t new_capacity) {
        if (new_capacity > capacity_) {
            int* new_data = new int[new_capacity];
            if (data) {
                std::copy(data, data + size_, new_data);
                delete[] data;
            }
            data = new_data;
            capacity_ = new_capacity;
        }
    }
    
    void shrink_to_fit() {
        if (capacity_ > size_) {
            if (size_ == 0) {
                delete[] data;
                data = nullptr;
                capacity_ = 0;
            } else {
                int* new_data = new int[size_];
                std::copy(data, data + size_, new_data);
                delete[] data;
                data = new_data;
                capacity_ = size_;
            }
        }
    }
    
    
    void clear() {
        size_ = 0;
    }
    
    void push_back(int value) {
        if (size_ == capacity_) {
            std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reserve(new_capacity);
        }
        data[size_++] = value;
    }
    
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }
    
    void insert(std::size_t pos, int value) {
        if (pos > size_) {
            throw std::out_of_range("Index not in range");
        }
        
        if (size_ == capacity_) {
            std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reserve(new_capacity);
        }
        
        for (std::size_t i = size_; i > pos; --i) {
            data[i] = data[i - 1];
        }
        
        data[pos] = value;
        ++size_;
    }
    
    void erase(std::size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Position out of range");
        }
        
        for (std::size_t i = pos; i < size_ - 1; ++i) {
            data[i] = data[i + 1];
        }
        
        --size_;
    }
    
    void resize(std::size_t new_size, int value = 0) {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        
        if (new_size > size_) {
            std::fill(data + size_, data + new_size, value);
        }
        
        size_ = new_size;
    }
    
    void swap(VectorInt& other) noexcept {
        std::swap(data, other.data);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    
    
    int* begin() {
        return data;
    }
    
    const int* begin() const {
        return data;
    }
    
    int* end() {
        return data + size_;
    }
    
    const int* end() const {
        return data + size_;
    }
};

bool operator==(const VectorInt& lhs, const VectorInt& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (std::size_t i = 0; i < lhs.size(); ++i) {
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