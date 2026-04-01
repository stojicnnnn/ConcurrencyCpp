#include <gtest/gtest.h>
#include "vector_implementation.hpp"
#include <vector>

// -------- VectorIntTest_Constructors --------

TEST(VectorIntTest_Constructors, DefaultConstructor) {
    VectorInt v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorIntTest_Constructors, SizeConstructor) {
    VectorInt v(5);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

TEST(VectorIntTest_Constructors, SizeAndValueConstructor) {
    VectorInt v(5, 42);
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

TEST(VectorIntTest_Constructors, InitializerListConstructor) {
    VectorInt v = {1, 2, 3, 4, 5};
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[4], 5);
}

// -------- VectorIntTest_CopyAndMove --------

TEST(VectorIntTest_CopyAndMove, CopyConstructor) {
    VectorInt v1 = {1, 2, 3};
    VectorInt v2(v1);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1, v2);
    // Modifying v2 should not affect v1
    v2[0] = 10;
    EXPECT_NE(v1[0], v2[0]);
}

TEST(VectorIntTest_CopyAndMove, CopyAssignment) {
    VectorInt v1 = {1, 2, 3};
    VectorInt v2;
    v2 = v1;
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1, v2);
    // Modifying v2 should not affect v1
    v2[0] = 10;
    EXPECT_NE(v1[0], v2[0]);
    
    // Self-assignment
    v2 = v2;
    EXPECT_EQ(v2[0], 10);
    EXPECT_EQ(v2.size(), 3);
}

TEST(VectorIntTest_CopyAndMove, MoveConstructor) {
    VectorInt v1 = {1, 2, 3};
    const int* data_ptr = v1.data();
    VectorInt v2(std::move(v1));
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2.data(), data_ptr);
    EXPECT_EQ(v1.data(), nullptr);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);
}

TEST(VectorIntTest_CopyAndMove, MoveAssignment) {
    VectorInt v1 = {1, 2, 3};
    VectorInt v2 = {4, 5};
    const int* data_ptr = v1.data();
    v2 = std::move(v1);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2.data(), data_ptr);
    EXPECT_EQ(v1.data(), nullptr);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);
    
    // Self-assignment
    v2 = std::move(v2);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_NE(v2.data(), nullptr);
}

// -------- VectorIntTest_ElementAccess --------

TEST(VectorIntTest_ElementAccess, At) {
    VectorInt v = {1, 2, 3};
    EXPECT_EQ(v.at(1), 2);
    v.at(1) = 10;
    EXPECT_EQ(v.at(1), 10);
    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST(VectorIntTest_ElementAccess, OperatorBracket) {
    VectorInt v = {1, 2, 3};
    EXPECT_EQ(v[1], 2);
    v[1] = 10;
    EXPECT_EQ(v[1], 10);
}

TEST(VectorIntTest_ElementAccess, FrontAndBack) {
    VectorInt v = {10, 20, 30};
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);
    v.front() = 100;
    v.back() = 300;
    EXPECT_EQ(v[0], 100);
    EXPECT_EQ(v[2], 300);
}

TEST(VectorIntTest_ElementAccess, Data) {
    VectorInt v = {1, 2, 3};
    int* ptr = v.data();
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 1);
    *ptr = 10;
    EXPECT_EQ(v[0], 10);
}

// -------- VectorIntTest_Modifiers --------

TEST(VectorIntTest_Modifiers, PushBack) {
    VectorInt v;
    v.push_back(1);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
    
    v.push_back(2);
    v.push_back(3);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[2], 3);
    EXPECT_GE(v.capacity(), 3);
}

TEST(VectorIntTest_Modifiers, PopBack) {
    VectorInt v = {1, 2, 3};
    v.pop_back();
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), 2);
    
    v.pop_back();
    v.pop_back();
    EXPECT_TRUE(v.empty());
    // Safe on empty
    v.pop_back();
    EXPECT_TRUE(v.empty());
}

TEST(VectorIntTest_Modifiers, Insert) {
    VectorInt v = {1, 3};
    v.insert(1, 2);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    
    v.insert(0, 0);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v.size(), 4);
    
    v.insert(4, 4);
    EXPECT_EQ(v[4], 4);
    EXPECT_EQ(v.size(), 5);
    
    EXPECT_THROW(v.insert(10, 10), std::out_of_range);
}

TEST(VectorIntTest_Modifiers, Erase) {
    VectorInt v = {1, 2, 3, 4};
    v.erase(1);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);
    
    EXPECT_THROW(v.erase(10), std::out_of_range);
}

TEST(VectorIntTest_Modifiers, Resize) {
    VectorInt v = {1, 2, 3};
    v.resize(5, 10);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[3], 10);
    EXPECT_EQ(v[4], 10);
    
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST(VectorIntTest_Modifiers, Swap) {
    VectorInt v1 = {1, 2};
    VectorInt v2 = {3, 4, 5};
    v1.swap(v2);
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v1[0], 3);
    EXPECT_EQ(v2[0], 1);
}

TEST(VectorIntTest_Modifiers, Clear) {
    VectorInt v = {1, 2, 3};
    size_t cap = v.capacity();
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), cap);
}

// -------- VectorIntTest_Capacity --------

TEST(VectorIntTest_Capacity, Reserve) {
    VectorInt v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
    EXPECT_TRUE(v.empty());
    
    size_t cap = v.capacity();
    v.reserve(5); // Should do nothing since 5 < 10
    EXPECT_EQ(v.capacity(), cap);
}

TEST(VectorIntTest_Capacity, ShrinkToFit) {
    VectorInt v(100);
    v.resize(10);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 10);
    EXPECT_EQ(v.size(), 10);
    
    v.clear();
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.size(), 0);
}

// -------- VectorIntTest_OperatorsAndIterators --------

TEST(VectorIntTest_Operators, Equality) {
    VectorInt v1 = {1, 2, 3};
    VectorInt v2 = {1, 2, 3};
    VectorInt v3 = {1, 2, 4};
    VectorInt v4 = {1, 2};
    
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
    
    EXPECT_FALSE(v1 == v3);
    EXPECT_TRUE(v1 != v3);
    
    EXPECT_FALSE(v1 == v4);
    EXPECT_TRUE(v1 != v4);
}

TEST(VectorIntTest_Iterators, BeginEnd) {
    VectorInt v = {1, 2, 3};
    int sum = 0;
    for (int* it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
    
    sum = 0;
    for (auto val : v) {
        sum += val;
    }
    EXPECT_EQ(sum, 6);
}
