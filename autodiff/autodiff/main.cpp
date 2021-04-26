//
//  main.cpp
//  autodiff
//
//  Created by tuRen on 2021/3/3.
//

#include <iostream>
#include "forward.hpp"

typedef double real;
int main(int argc, const char * argv[]) {
    varelect<real> x(0, 'x');
    varelect<real> y(0, 'y');
    
    x = 3;
    y = 4;
    
    auto f = x * x * y + y + 2;
    std::cout << "test func: " << f.description() << std::endl;
    std::cout << "test func: f(3, 4) = " << f.value << std::endl;
    
    auto dfdx = f.gradient(x);
    auto dfdy = f.gradient(y);
    std::cout << "test func: f(3, 4) dfdx: " << dfdx.value << " dfdy: " << dfdy.value << std::endl;
    
    auto d2fdxdx = dfdx.gradient(x);
    auto d2fdxdy = dfdx.gradient(y);
    
    auto d2fdydx = dfdy.gradient(x);
    auto d2fdydy = dfdy.gradient(y);
    std::cout << "test func: f(3, 4) d2fdxdx: " << d2fdxdx.value << " d2fdxdy: " << d2fdxdy.value << std::endl;
    std::cout << "test func: f(3, 4) d2fdydx: " << d2fdydx.value << " d2fdydy: " << d2fdydy.value << std::endl;
    
    return 0;
}

//    f->backpropagate(1.0);
//    std::cout << "test func backpropagate: x gradient: " << x->_gradient << " y gradient: " << y->_gradient << std::endl;

//  cli former output:
//    test func: ((x) * (x)) * (y) + y + 2.000000
//    test func: f(3, 4) = 42
//    test func: f(3, 4) dfdx: 24 dfdy: 10
//    test func: f(3, 4) d2fdxdx: 8 d2fdxdy: 6
//    test func: f(3, 4) d2fdydx: 6 d2fdydy: 0
//    test func backpropagate: x gradient: 24 y gradient: 10
