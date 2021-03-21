//
//  main.cpp
//  autodiff
//
//  Created by tuRen on 2021/3/3.
//

#include <iostream>
#include <vector>
#include <functional>

#define EPS 0.0001

double func(double x, double y) {
    return x * x * y + y + 2;
}

std::vector<double> gradients(int argc, std::function<double(double, double, double)> fp) {
    std::vector<double> ans;
        
    double base_func_value = fp(0, 0, 0);
    for(int i = 0; i < argc; i++) {
        std::vector<double> tweaked_params(3, 0);
        tweaked_params[i] = EPS;
        
        double tweaked_func_value = fp(tweaked_params[0], tweaked_params[1], tweaked_params[2]);
        double derivative = (tweaked_func_value - base_func_value)/EPS;
        ans.push_back(derivative);
    }
    
    return ans;
}

std::vector<double> df(double x, double y) {
    auto lam = [=](double lx, double ly, double o) -> double {
        return func(x + lx, y + ly);
    };
    return gradients(2, lam);
}

double dfdx(double x, double y) {
    return df(x, y)[0];
}

double dfdy(double x, double y) {
    return df(x, y)[1];
}

std::vector<double> d2f(double x, double y) {
    auto lam_x = [=](double lx, double ly, double o) -> double {
        return dfdx(x + lx, y + ly);
    };
    std::vector<double> ans_dx = gradients(2, lam_x);
    
    auto lam_y = [=](double lx, double ly, double o) -> double {
        return dfdy(x + lx, y + ly);
    };
    std::vector<double> ans_dy = gradients(2, lam_y);
    
    ans_dx.insert(ans_dx.end(), ans_dy.begin(), ans_dy.end());
    return ans_dx;
}

int tmain(int argc, const char * argv[]) {
    double fdx = dfdx(3.0f, 4.0f);
    double fdy = dfdy(3.0f, 4.0f);
    
    std::cout << "dfdx: " << fdx << " dfdy: " << fdy << std::endl;
    
    std::vector<double> ds = d2f(3.0f, 4.0f);
    
    std::cout << "get result:" << std::endl;
    for(auto f: ds) {
        std::cout << f << std::endl;
    }
    
    return 0;
}

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

double z(double x) {
    return sin(pow(x, 2));
}

int zmain(int argc, const char * argv[]) {
    auto lam = [=](double lx, double o, double e) -> double {
        return z(3 + lx);
    };
    
    std::vector<double> ans = gradients(1, lam);

    std::cout << "test func: dz = " << ans[0] << std::endl;
    
    return 0;
}
