//
// Created by Android on 10.10.2020.
//

#include <gtest/gtest.h>
#include "map_allocator.h"

TEST(MapSize, True) {
    auto m = std::map<int,int,std::less<int>,
        map_allocator<std::pair<const int, int>, 10>
     >{};

    for (int i = 0; i < 10; ++i)
        m[i] = i;

    ASSERT_TRUE(m.size() == 10);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
