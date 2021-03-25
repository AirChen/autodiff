//
//  TemplateTest.cpp
//  tests
//
//  Created by AirChen on 2021/3/26.
//

#include "TemplateTest.hpp"
#include "TemplateUtils.h"
#include "gtest/gtest.h"

class TemplateTest : public ::testing::Test {
protected:

    TemplateTest() {
     // You can do set-up work for each test here.
    }

    ~TemplateTest() override {
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
};

int basicFunc(int x, int y, int z, int a) {
    std::cout << x << " + " << y << " + " << z << " + " << a << std::endl;
    return x + y + z + a;
}

TEST_F(TemplateTest, BasicFunction) {
    int v[] = { 100,200,300,400,500,600 };
    int result = call_func<int, decltype(basicFunc), int, int, int, int>(basicFunc, v);
    EXPECT_EQ(result, 1400);
}

double func(double x, double y) {
    return x * x * y + y + 2;
}

std::vector<double> df(double x, double y) {
    return gradients<double>(func, x, y);
}

double dfdx(double x, double y) {
    return df(x, y)[0];
}

double dfdy(double x, double y) {
    return df(x, y)[1];
}

std::vector<double> d2f(double x, double y) {
    auto ans_dx = gradients<double>(dfdx, x, y);
    auto ans_dy = gradients<double>(dfdy, x, y);

    ans_dx.insert(ans_dx.end(), ans_dy.begin(), ans_dy.end());
    return ans_dx;
}

TEST_F(TemplateTest, MulGradients) {
    double fdx = dfdx(3.0f, 4.0f);
    double fdy = dfdy(3.0f, 4.0f);

    std::cout << "dfdx: " << fdx << " dfdy: " << fdy << std::endl;

    std::vector<double> ds = d2f(3.0f, 4.0f);

    std::cout << "get result:" << std::endl;
    for(auto f: ds) {
        std::cout << f << std::endl;
    }
}

double z(double x) {
    return sin(pow(x, 2));
}

TEST_F(TemplateTest, SinPowGradients) {
    std::vector<double> ans = gradients<double>(z, 3.0f);

    std::cout << "test func: dz = " << ans[0] << std::endl;
}
