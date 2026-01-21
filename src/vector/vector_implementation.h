#include <stdexcept>

class VectorInt{
    int * data;
    std::size_t size;
    std::size_t capacity;

    //default constructor destructor
    VectorInt() : data(nullptr), size(0), capacity(0) {}
    ~VectorInt(){delete[] data;}
    //copy constructor
    VectorInt(const VectorInt& other) : data(new int(*other.data));
    //copy assignment
    VectorInt& operator=(const VectorInt& other){
        if(this != &other)
        {
            delete[] data;
            data = new(int*(other.data));
        }
        return *this;
    }
    //move constructor
    VectorInt(VectorInt&& other) : data = other.data {
        other.data = nullptr;
    }
    //move assignment
    VectorInt& operator=(VectorInt&& other)
    {   if(this != other)
        {
        delete[] data;
        data = other.data;
        oher.data = nullptr;
        }
        return *this;
    }

public:

