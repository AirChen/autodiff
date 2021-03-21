//
//  main.cpp
//  tests
//
//  Created by AirChen on 2021/3/22.
//

#include <iostream>
#include "ToyDatas.hpp"
#include "gtest/gtest.h"

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  FooTest() {
     // You can do set-up work for each test here.
  }

  ~FooTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(FooTest, BasicTest) {
    auto x = var_func("x");
    auto y = var_func("y");
    auto c = const_func(2.0);

    auto f = add_func(mul_func(mul_func(x, x), y), add_func(y, c));    
    ASSERT_STRCASEEQ(f->str().data(), "((x) * (x)) * (y) + y + 2.000000");

    x->setValue(3.0);
    y->setValue(4.0);
    EXPECT_EQ(f->evaluate(), 42);

    auto dfdx = f->gradient(x);
    auto dfdy = f->gradient(y);
    EXPECT_EQ(dfdx->evaluate(), 24);
    EXPECT_EQ(dfdy->evaluate(), 10);

    auto d2fdxdx = dfdx->gradient(x);
    auto d2fdxdy = dfdx->gradient(y);

    auto d2fdydx = dfdy->gradient(x);
    EXPECT_EQ(d2fdxdx->evaluate(), 8);
    EXPECT_EQ(d2fdxdy->evaluate(), 6);
    
    auto d2fdydy = dfdy->gradient(y);
    EXPECT_EQ(d2fdydx->evaluate(), 6);
    EXPECT_EQ(d2fdydy->evaluate(), 0);

    f->backpropagate(1.0);
    EXPECT_EQ(x->_gradient, 24);
    EXPECT_EQ(y->_gradient, 10);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

