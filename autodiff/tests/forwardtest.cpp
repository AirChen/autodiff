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
    elect<double> x = 100;
    elect<double> y = 200;
};

using namespace compare;
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

using namespace fake;
TEST_F(ForwardTest, aliasing_tests)
{
    x = 1; x = x + 3*x - 2*x + x;
    EXPECT_TRUE(x == 3);
    std::cout << "x = " << x.description() << std::endl;

    x = 1; x += x + 3*x - 2*x + x;
    EXPECT_TRUE(x == 4);
    std::cout << "x = " << x.description() << std::endl;

    x = 1; x -= x + 3*x - 2*x + x;
    EXPECT_TRUE(x == -2);
    std::cout << "x = " << x.description() << std::endl;

    x = 1; x *= x + 3*x - 2*x + x;
    EXPECT_TRUE(x == 3);
    std::cout << "x = " << x.description() << std::endl;

    x = 1; x /= x + x;
    EXPECT_TRUE(x == 0.5);
    std::cout << "x = " << x.description() << std::endl;
}

TEST_F(ForwardTest, comparison_operators)
{
    x = 6;
    y = 5;
    
    EXPECT_TRUE(x == 6);
    EXPECT_TRUE(6 == x);
    EXPECT_TRUE(x == x);
    
    EXPECT_TRUE(x != 5);
    EXPECT_TRUE(5 != x);
    EXPECT_TRUE(x != y);
    
    EXPECT_TRUE(x > 5);
    EXPECT_TRUE(x > y);
    
    EXPECT_TRUE(x >= 6);
    EXPECT_TRUE(x >= x);
    EXPECT_TRUE(x >= y);
    
    EXPECT_TRUE(5 < x);
    EXPECT_TRUE(y < x);
    
    EXPECT_TRUE(6 <= x);
    EXPECT_TRUE(x <= x);
    EXPECT_TRUE(y <= x);
}

TEST_F(ForwardTest, forward_gradient)
{
    x = 6;
    elect<double> f = 2 * x + 100;
    std::cout << f.gradient(x).description() << std::endl;
    std::cout << f.gradient(x).value << std::endl;
    
    elect<double> f2 = 2 * x * x + 100 * x;
    elect<double> fg = f2.gradient(x);
    std::cout << fg.description() << std::endl;
    std::cout << fg.value << std::endl;
    
    elect<double> fgg = fg.gradient(x);
    std::cout << fgg.description() << std::endl;
    std::cout << fgg.value << std::endl;
    
    elect<double> f3 = x/x;
    elect<double> f3g = f3.gradient(x); // error!
    std::cout << f3g.description() << std::endl;
    std::cout << f3g.value << std::endl;
}
