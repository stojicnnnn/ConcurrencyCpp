```cpp
#include "VectorInt.hpp"
#include <iostream>

void print_vector(const VectorInt& v) {
    std::cout << "size: " << v.size() << " cap: " << v.capacity() << " -> ";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "testing default constructor" << std::endl;
    VectorInt v1;
    print_vector(v1);
    
    std::cout << "\npush_back test" << std::endl;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);
    v1.push_back(40);
    v1.push_back(50);
    print_vector(v1);
    
    std::cout << "\npop_back" << std::endl;
    v1.pop_back();
    v1.pop_back();
    print_vector(v1);
    
    std::cout << "\naccess stuff" << std::endl;
    std::cout << "v1[0] = " << v1[0] << std::endl;
    std::cout << "v1[1] = " << v1[1] << std::endl;
    std::cout << "front = " << v1.front() << std::endl;
    std::cout << "back = " << v1.back() << std::endl;
    std::cout << "at(1) = " << v1.at(1) << std::endl;
    
    std::cout << "\nconstructor with size" << std::endl;
    VectorInt v2(5, 100);
    print_vector(v2);
    
    std::cout << "\ninitializer list" << std::endl;
    VectorInt v3 = {1, 2, 3, 4, 5};
    print_vector(v3);
    
    std::cout << "\ncopy constructor" << std::endl;
    VectorInt v4 = v3;
    print_vector(v4);
    v3[0] = 999;
    std::cout << "changed v3[0] to 999" << std::endl;
    std::cout << "v3: "; print_vector(v3);
    std::cout << "v4: "; print_vector(v4);
    
    std::cout << "\ncopy assignment" << std::endl;
    VectorInt v5;
    v5 = v1;
    print_vector(v5);
    
    std::cout << "\nmove constructor" << std::endl;
    VectorInt v6 = std::move(v2);
    std::cout << "v6: "; print_vector(v6);
    std::cout << "v2 after: "; print_vector(v2);
    
    std::cout << "\nmove assignment" << std::endl;
    VectorInt v7;
    v7 = std::move(v6);
    std::cout << "v7: "; print_vector(v7);
    std::cout << "v6 after: "; print_vector(v6);
    
    std::cout << "\ninsert test" << std::endl;
    VectorInt v8 = {1, 2, 4, 5};
    print_vector(v8);
    v8.insert(2, 3);
    std::cout << "inserted 3 at pos 2: "; print_vector(v8);
    v8.insert(0, 0);
    std::cout << "inserted 0 at pos 0: "; print_vector(v8);
    
    std::cout << "\nerase test" << std::endl;
    v8.erase(0);
    std::cout << "erased pos 0: "; print_vector(v8);
    v8.erase(2);
    std::cout << "erased pos 2: "; print_vector(v8);
    
    std::cout << "\nresize" << std::endl;
    VectorInt v9 = {1, 2, 3};
    print_vector(v9);
    v9.resize(5, 99);
    std::cout << "resize to 5: "; print_vector(v9);
    v9.resize(2);
    std::cout << "resize to 2: "; print_vector(v9);
    
    std::cout << "\nreserve" << std::endl;
    VectorInt v10;
    std::cout << "before: "; print_vector(v10);
    v10.reserve(10);
    std::cout << "after reserve(10): "; print_vector(v10);
    v10.push_back(1);
    v10.push_back(2);
    std::cout << "added stuff: "; print_vector(v10);
    
    std::cout << "\nclear" << std::endl;
    VectorInt v11 = {1, 2, 3, 4, 5};
    print_vector(v11);
    v11.clear();
    std::cout << "after clear: "; print_vector(v11);
    
    std::cout << "\nempty check" << std::endl;
    std::cout << "v11 empty? " << (v11.empty() ? "yes" : "no") << std::endl;
    std::cout << "v1 empty? " << (v1.empty() ? "yes" : "no") << std::endl;
    
    std::cout << "\nrange for loop" << std::endl;
    VectorInt v12 = {10, 20, 30, 40, 50};
    for (int val : v12) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\ncomparison operators" << std::endl;
    VectorInt v13 = {1, 2, 3};
    VectorInt v14 = {1, 2, 3};
    VectorInt v15 = {1, 2, 4};
    std::cout << "v13 == v14: " << (v13 == v14) << std::endl;
    std::cout << "v13 == v15: " << (v13 == v15) << std::endl;
    std::cout << "v13 != v15: " << (v13 != v15) << std::endl;
    
    std::cout << "\nexception test" << std::endl;
    VectorInt v16 = {1, 2, 3};
    try {
        std::cout << "accessing at(10)..." << std::endl;
        v16.at(10);
    } catch (const std::out_of_range& e) {
        std::cout << "got exception: " << e.what() << std::endl;
    }
    
    std::cout << "\ndone" << std::endl;
    
    return 0;
}
