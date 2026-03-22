#include <gtest/gtest.h>
#include "vector_implementation.hpp"
#include <vector>
#include <string>


TEST(VectorStringTest_Constructors, DefaultConstructor) {
    VectorString v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorStringTest_Constructors, SizeConstructor) {
    VectorString v(5);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], "");
    }
}

TEST(VectorStringTest_Constructors, SizeAndValueConstructor) {
    VectorString v(5, "hello");
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], "hello");
    }
}

TEST(VectorStringTest_Constructors, InitializerListConstructor) {
    VectorString v = {"a", "b", "c", "d", "e"};
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[4], "e");
}


TEST(VectorStringTest_CopyAndMove, CopyConstructor) {
    VectorString v1 = {"a", "b", "c"};
    VectorString v2(v1);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1, v2);
    v2[0] = "x";
    EXPECT_NE(v1[0], v2[0]);
}

TEST(VectorStringTest_CopyAndMove, CopyAssignment) {
    VectorString v1 = {"a", "b", "c"};
    VectorString v2;
    v2 = v1;
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1, v2);
    v2[0] = "x";
    EXPECT_NE(v1[0], v2[0]);
    
    v2 = v2;
    EXPECT_EQ(v2[0], "x");
    EXPECT_EQ(v2.size(), 3);
}

TEST(VectorStringTest_CopyAndMove, MoveConstructor) {
    VectorString v1 = {"a", "b", "c"};
    const std::string* data_ptr = v1.data();
    VectorString v2(std::move(v1));
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2.data(), data_ptr);
    EXPECT_EQ(v1.data(), nullptr);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);
}

TEST(VectorStringTest_CopyAndMove, MoveAssignment) {
    VectorString v1 = {"a", "b", "c"};
    VectorString v2 = {"x", "y"};
    const std::string* data_ptr = v1.data();
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


TEST(VectorStringTest_ElementAccess, At) {
    VectorString v = {"a", "b", "c"};
    EXPECT_EQ(v.at(1), "b");
    v.at(1) = "x";
    EXPECT_EQ(v.at(1), "x");
    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST(VectorStringTest_ElementAccess, OperatorBracket) {
    VectorString v = {"a", "b", "c"};
    EXPECT_EQ(v[1], "b");
    v[1] = "x";
    EXPECT_EQ(v[1], "x");
}

TEST(VectorStringTest_ElementAccess, FrontAndBack) {
    VectorString v = {"a", "b", "c"};
    EXPECT_EQ(v.front(), "a");
    EXPECT_EQ(v.back(), "c");
    v.front() = "x";
    v.back() = "z";
    EXPECT_EQ(v[0], "x");
    EXPECT_EQ(v[2], "z");
}

TEST(VectorStringTest_ElementAccess, Data) {
    VectorString v = {"a", "b", "c"};
    std::string* ptr = v.data();
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, "a");
    *ptr = "x";
    EXPECT_EQ(v[0], "x");
}


TEST(VectorStringTest_Modifiers, PushBack) {
    VectorString v;
    v.push_back("a");
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "a");
    
    v.push_back("b");
    v.push_back("c");
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[2], "c");
    EXPECT_GE(v.capacity(), 3);
}

TEST(VectorStringTest_Modifiers, PopBack) {
    VectorString v = {"a", "b", "c"};
    v.pop_back();
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), "b");
    
    v.pop_back();
    v.pop_back();
    EXPECT_TRUE(v.empty());
    // Safe on empty
    v.pop_back();
    EXPECT_TRUE(v.empty());
}

TEST(VectorStringTest_Modifiers, Insert) {
    VectorString v = {"a", "c"};
    v.insert(1, "b");
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "b");
    EXPECT_EQ(v[2], "c");
    
    v.insert(0, "0");
    EXPECT_EQ(v[0], "0");
    EXPECT_EQ(v.size(), 4);
    
    v.insert(4, "d");
    EXPECT_EQ(v[4], "d");
    EXPECT_EQ(v.size(), 5);
    
    EXPECT_THROW(v.insert(10, "x"), std::out_of_range);
}

TEST(VectorStringTest_Modifiers, Erase) {
    VectorString v = {"a", "b", "c", "d"};
    v.erase(1);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "c");
    EXPECT_EQ(v[2], "d");
    
    EXPECT_THROW(v.erase(10), std::out_of_range);
}

TEST(VectorStringTest_Modifiers, Resize) {
    VectorString v = {"a", "b", "c"};
    v.resize(5, "x");
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[3], "x");
    EXPECT_EQ(v[4], "x");
    
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "b");
}

TEST(VectorStringTest_Modifiers, Swap) {
    VectorString v1 = {"a", "b"};
    VectorString v2 = {"c", "d", "e"};
    v1.swap(v2);
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v1[0], "c");
    EXPECT_EQ(v2[0], "a");
}

TEST(VectorStringTest_Modifiers, Clear) {
    VectorString v = {"a", "b", "c"};
    size_t cap = v.capacity();
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), cap);
}


TEST(VectorStringTest_Capacity, Reserve) {
    VectorString v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
    EXPECT_TRUE(v.empty());
    
    size_t cap = v.capacity();
    v.reserve(5);
    EXPECT_EQ(v.capacity(), cap);
}

TEST(VectorStringTest_Capacity, ShrinkToFit) {
    VectorString v(100);
    v.resize(10);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 10);
    EXPECT_EQ(v.size(), 10);
    
    v.clear();
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.size(), 0);
}


TEST(VectorStringTest_Operators, Equality) {
    VectorString v1 = {"a", "b", "c"};
    VectorString v2 = {"a", "b", "c"};
    VectorString v3 = {"a", "b", "d"};
    VectorString v4 = {"a", "b"};
    
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
    
    EXPECT_FALSE(v1 == v3);
    EXPECT_TRUE(v1 != v3);
    
    EXPECT_FALSE(v1 == v4);
    EXPECT_TRUE(v1 != v4);
}

TEST(VectorStringTest_Iterators, BeginEnd) {
    VectorString v = {"a", "b", "c"};
    std::string sum = "";
    for (std::string* it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, "abc");
    
    sum = "";
    for (const auto& val : v) {
        sum += val;
    }
    EXPECT_EQ(sum, "abc");
}
