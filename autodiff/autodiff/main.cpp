//
//  main.cpp
//  autodiff
//
//  Created by tuRen on 2021/3/3.
//

#include <iostream>
#include "ToyDatas.hpp"

int main(int argc, const char * argv[]) {
    auto x = var_func("x");
    auto y = var_func("y");
    auto c = const_func(2.0);
    
    auto f = add_func(mul_func(mul_func(x, x), y), add_func(y, c));
    
    std::cout << "test func: " << f->str() << std::endl;
    
    x->setValue(3.0);
    y->setValue(4.0);
    std::cout << "test func: f(3, 4) = " << f->evaluate() << std::endl;
    
    auto dfdx = f->gradient(x);
    auto dfdy = f->gradient(y);
    std::cout << "test func: f(3, 4) dfdx: " << dfdx->evaluate() << " dfdy: " << dfdy->evaluate() << std::endl;
    
    auto d2fdxdx = dfdx->gradient(x);
    auto d2fdxdy = dfdx->gradient(y);
    
    auto d2fdydx = dfdy->gradient(x);
    auto d2fdydy = dfdy->gradient(y);
    std::cout << "test func: f(3, 4) d2fdxdx: " << d2fdxdx->evaluate() << " d2fdxdy: " << d2fdxdy->evaluate() << std::endl;
    std::cout << "test func: f(3, 4) d2fdydx: " << d2fdydx->evaluate() << " d2fdydy: " << d2fdydy->evaluate() << std::endl;
    
    f->backpropagate(1.0);
    std::cout << "test func backpropagate: x gradient: " << x->_gradient << " y gradient: " << y->_gradient << std::endl;
    
    return 0;
}
