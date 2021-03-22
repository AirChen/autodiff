//
//  MatrixTest.cpp
//  tests
//
//  Created by AirChen on 2021/3/22.
//

#include "MatrixTest.hpp"
#include "gtest/gtest.h"
#include <eigen3/Eigen/Dense>

// The fixture for testing class Foo.
class MatrixTest : public ::testing::Test {
protected:

    MatrixTest() {
     // You can do set-up work for each test here.
    }

    ~MatrixTest() override {
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

using namespace Eigen;
TEST_F(MatrixTest, BasicTest) {
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    std::cout << m << std::endl;
    
    m = MatrixXd::Random(3,3);
    m = (m + MatrixXd::Constant(3,3,1.2)) * 50;
    std::cout << "m =" << std::endl << m << std::endl;
    VectorXd v(3);
    v << 1, 2, 3;
    std::cout << "m * v =" << std::endl << m * v << std::endl;
    
    m = Matrix3d::Random();
    m = (m + Matrix3d::Constant(1.2)) * 50;
    std::cout << "m =" << std::endl << m << std::endl;
    Vector3d v1(1,2,3);

    std::cout << "m * v =" << std::endl << m * v1 << std::endl;
}

TEST_F(MatrixTest, Resizing) {
    MatrixXd m(2,5);
    m.resize(4,3);
    std::cout << "The matrix m is of size "
            << m.rows() << "x" << m.cols() << std::endl;
    std::cout << "It has " << m.size() << " coefficients" << std::endl;
    
    VectorXd v(2);
    v.resize(5);
    std::cout << "The vector v is of size " << v.size() << std::endl;
    std::cout << "As a matrix, v is of size "
            << v.rows() << "x" << v.cols() << std::endl;
}

TEST_F(MatrixTest, Resizing_1) {
    MatrixXf a(2,2);
    std::cout << "a is of size " << a.rows() << "x" << a.cols() << std::endl;
    MatrixXf b(3,3);
    a = b;
    std::cout << "a is now of size " << a.rows() << "x" << a.cols() << std::endl;
}
