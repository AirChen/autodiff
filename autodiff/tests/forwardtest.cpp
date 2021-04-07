//
//  forwardtest.cpp
//  tests
//
//  Created by AirChen on 2021/3/28.
//

#include "forwardtest.hpp"
#include "forward.hpp"
#include "gtest/gtest.h"

class ForwardTest: public ::testing::Test
{
public:
    std::shared_ptr<elect<double>> x = std::make_shared<elect<double>>(100);
    std::shared_ptr<elect<double>> y = std::make_shared<elect<double>>(200);
};

using namespace fake;
TEST_F(ForwardTest, trivial_tests)
{
    EXPECT_TRUE(x == 100);
    x += 1;
    EXPECT_TRUE(x == 101);
    x -= 1;
    EXPECT_TRUE(x == 100);
    x *= 2;
    EXPECT_TRUE(x == 200);
    x /= 10;
    EXPECT_TRUE(x == 20);
};

TEST_F(ForwardTest, aliasing_tests)
{
    x = 1; x = x + 3.0*x - 2.0*x + x;
    EXPECT_TRUE(x == 3);

    x = 1; x += x + 3.0*x - 2.0*x + x;
    EXPECT_TRUE(x == 4);

    x = 1; x -= x + 3.0*x - 2.0*x + x;
    EXPECT_TRUE(x == -2);

    x = 1; x *= x + 3.0*x - 2.0*x + x;
    EXPECT_TRUE(x == 3);

    x = 1; x /= x + x;
    EXPECT_TRUE(x == 0.5);
}
