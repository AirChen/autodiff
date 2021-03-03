//
//  main.cpp
//  autodiff
//
//  Created by tuRen on 2021/3/3.
//

#include <iostream>
#include <vector>

#define EPS 0.0001

typedef float (*funcptr) (...);

float func(float x, float y) {
    std::cout << "input params:" << x << " " << y << std::endl;
    
    return x * x * y + y + 2;
}

std::vector<float> gradients(funcptr fptr, int argc, ...) {
    std::vector<float> ans;
    std::vector<float> params;
    va_list args;
    va_start(args, argc);
    
    std::cout << "size of float: " << sizeof(float) << std::endl;
    std::cout << "size of double: " << sizeof(double) << std::endl;
    std::cout << "size of int: " << sizeof(int) << std::endl;
    std::cout << "size of long: " << sizeof(long) << std::endl;
    
    for (int i = 0; i < argc; i++) {
        params.push_back((float)va_arg(args, int));
    }
    
    va_end(args);
    
    if (params.size() == 2) {
        float base_func_value = fptr(params[0], params[1]);
        for(int i = 0; i < argc; i++) {
            std::vector<float> tweaked_params = params;
            
            float &v = tweaked_params[i];
            v += EPS;
            
            float tweaked_func_value = fptr(tweaked_params[0], tweaked_params[1]);
            float derivative = (tweaked_func_value - base_func_value)/EPS;
            
            ans.push_back(derivative);
        }
    }
    
    if (params.size() == 3) {
        float base_func_value = fptr(params[0], params[1], params[2]);
        for(int i = 0; i < argc; i++) {
            std::vector<float> tweaked_params = params;
            
            float &v = tweaked_params[i];
            v += EPS;
            
            float tweaked_func_value = fptr(tweaked_params[0], tweaked_params[1], tweaked_params[2]);
            float derivative = (tweaked_func_value - base_func_value)/EPS;
            
            ans.push_back(derivative);
        }
    }
    
    return ans;
}

std::vector<float> df(float x, float y) {
    return gradients((funcptr)func, 2, x, y);
}

int main(int argc, const char * argv[]) {
    std::vector<float> ds = df(3, 4);
    
    std::cout << "get result:" << std::endl;
    for(auto f: ds) {
        std::cout << f << std::endl;
    }
    
    return 0;
}
