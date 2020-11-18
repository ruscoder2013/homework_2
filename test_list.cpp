//
// Created by Android on 10.10.2020.
//

#include <gtest/gtest.h>
#include "map_allocator.h"

TEST(List, PushBack) {
    ASSERT_EQ(1, 1);
}

TEST(Filter, True) {
    auto m = std::map<int,int,std::less<int>,
        map_allocator<std::pair<const int, int>, 10>
     >{};

    for (int i = 0; i < 10; ++i)
        m[i] = i;

    ASSERT_TRUE(m.size() == 10);
}
/*
TEST(Filter_46_70, True) {
    std::vector<std::vector<int>> ip_pool;
    std::vector<int> vec_1{46, 70, 3, 4};
    std::vector<int> vec_2{46, 80, 3, 4};
    ip_pool.push_back(vec_1);
    ip_pool.push_back(vec_2);
    auto ip = filter(ip_pool, 46, 70);
    ASSERT_TRUE(ip.size() == 1);
    ASSERT_TRUE(std::find(ip.begin(), ip.end(), vec_1) != ip.end());
    ASSERT_TRUE(std::find(ip.begin(), ip.end(), vec_2) == ip.end());
}

TEST(FilterAny_46, True) {
    std::vector<std::vector<int>> ip_pool;
    std::vector<int> vec_1{46, 2, 3, 4};
    std::vector<int> vec_2{0, 46, 2, 4};
    std::vector<int> vec_3{1, 2, 46, 4};
    std::vector<int> vec_4{0, 1, 2, 46};
    std::vector<int> vec_5{1, 2, 3, 4};
    ip_pool.push_back(vec_1);
    ip_pool.push_back(vec_2);
    ip_pool.push_back(vec_3);
    ip_pool.push_back(vec_4);
    ip_pool.push_back(vec_5);

    auto ip = filter_any(ip_pool, 46);
    ASSERT_TRUE(ip.size() == 4);
    ASSERT_TRUE(std::find(ip.begin(), ip.end(), vec_1) != ip.end());
    ASSERT_TRUE(std::find(ip.begin(), ip.end(), vec_2) != ip.end());
    ASSERT_TRUE(std::find(ip.begin(), ip.end(), vec_3) != ip.end());
    ASSERT_TRUE(std::find(ip.begin(), ip.end(), vec_4) != ip.end());
    ASSERT_TRUE(std::find(ip.begin(), ip.end(), vec_5) == ip.end());
} */

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}