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

typedef double (*funcsubptr_2) (double x, double y);
typedef double (*funcsubptr_3) (double x, double y, double z);

double func(double x, double y) {
    return x * x * y + y + 2;
}

std::vector<double> gradients(funcptr fptr, int argc, double *args) {
    std::vector<double> ans;

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

int main(int argc, const char * argv[]) {
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
