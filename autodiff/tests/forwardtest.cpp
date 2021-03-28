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
    elect<int> x = 100;
    elect<int> y = 200;
};

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
//    x = 1; x = x + 3*x - 2*x + x;
//    REQUIRE( x == 3 );

//    x = 1; x += x + 3*x - 2*x + x;
//    REQUIRE( x == 4 );

//    x = 1; x -= x + 3*x - 2*x + x;
//    REQUIRE( x == -2 );

//    x = 1; x *= x + 3*x - 2*x + x;
//    REQUIRE( x == 3 );

    x = 1; x /= x + x;
//    REQUIRE( x == 0.5 );
}
