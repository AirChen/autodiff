//
//  main.cpp
//  autodiff
//
//  Created by tuRen on 2021/3/3.
//

#include <iostream>
#include <vector>

#define EPS 0.0001

typedef double (*funcptr) (...);

typedef double (*funcsubptr_1) (double x);
typedef double (*funcsubptr_2) (double x, double y);
typedef double (*funcsubptr_3) (double x, double y, double z);

double func(double x, double y) {
    return x * x * y + y + 2;
}

std::vector<double> gradients(funcptr fptr, int argc, double *args) {
    std::vector<double> ans;

    if (argc == 1) {
        funcsubptr_1 fp = (funcsubptr_1)fptr;
        
        double base_func_value = fp(args[0]);
        for(int i = 0; i < argc; i++) {
            std::vector<double> tweaked_params{args[0]};
            
            tweaked_params[i] += EPS;
            
            double tweaked_func_value = fp(tweaked_params[0]);
            double derivative = (tweaked_func_value - base_func_value)/EPS;
            
            ans.push_back(derivative);
        }
    }
    
    if (argc == 2) {
        funcsubptr_2 fp = (funcsubptr_2)fptr;
        
        double base_func_value = fp(args[0], args[1]);
        for(int i = 0; i < argc; i++) {
            std::vector<double> tweaked_params{args[0], args[1]};
            
            tweaked_params[i] += EPS;
            
            double tweaked_func_value = fp(tweaked_params[0], tweaked_params[1]);
            double derivative = (tweaked_func_value - base_func_value)/EPS;
            
            ans.push_back(derivative);
        }
    }
    
    if (argc == 3) {
        funcsubptr_3 fp = (funcsubptr_3)fptr;
        
        double base_func_value = fp(args[0], args[1], args[2]);
        for(int i = 0; i < argc; i++) {
            std::vector<double> tweaked_params{args[0], args[1], args[2]};

            tweaked_params[i] += EPS;

            double tweaked_func_value = fp(tweaked_params[0], tweaked_params[1], tweaked_params[2]);
            double derivative = (tweaked_func_value - base_func_value)/EPS;

            ans.push_back(derivative);
        }
    }
    
    return ans;
}

std::vector<double> df(double x, double y) {
    double vs[2] = {x, y};
    return gradients((funcptr)func, 2, vs);
}

double dfdx(double x, double y) {
    return df(x, y)[0];
}

double dfdy(double x, double y) {
    return df(x, y)[1];
}

std::vector<double> d2f(double x, double y) {
    double vs[2] = {x, y};
    std::vector<double> ans_dx = gradients((funcptr)dfdx, 2, vs);
    std::vector<double> ans_dy = gradients((funcptr)dfdy, 2, vs);
    
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
    auto x = Var("x");
    auto y = Var("y");
    auto c = Const(2.0);
    
    auto f = Add(Mul(Mul(x, x), y), Add(y, c));
    
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

int main_1(int argc, const char * argv[]) {
    double vs[1] = {3};
    std::vector<double> ans = gradients((funcptr)z, 1, vs);

    std::cout << "test func: dz = " << ans[0] << std::endl;
    
    return 0;
}
