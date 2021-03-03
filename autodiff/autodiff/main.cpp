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

typedef float (*funcsubptr_2) (float x, float y);
typedef float (*funcsubptr_3) (float x, float y, float z);

float func(float x, float y) {
    return x * x * y + y + 2;
}

std::vector<float> gradients(funcptr fptr, int argc, float *args) {
    std::vector<float> ans;

    if (argc == 2) {
        funcsubptr_2 fp = (funcsubptr_2)fptr;
        
        float base_func_value = fp(args[0], args[1]);
        for(int i = 0; i < argc; i++) {
            std::vector<float> tweaked_params{args[0], args[1]};
            
            tweaked_params[i] += EPS;
            
            float tweaked_func_value = fp(tweaked_params[0], tweaked_params[1]);
            float derivative = (tweaked_func_value - base_func_value)/EPS;
            
            ans.push_back(derivative);
        }
    }
    
    if (argc == 3) {
        funcsubptr_3 fp = (funcsubptr_3)fptr;
        
        float base_func_value = fp(args[0], args[1], args[2]);
        for(int i = 0; i < argc; i++) {
            std::vector<float> tweaked_params{args[0], args[1], args[2]};

            tweaked_params[i] += EPS;

            float tweaked_func_value = fp(tweaked_params[0], tweaked_params[1], tweaked_params[2]);
            float derivative = (tweaked_func_value - base_func_value)/EPS;

            ans.push_back(derivative);
        }
    }
    
    return ans;
}

std::vector<float> df(float x, float y) {
    float vs[2] = {x, y};
    return gradients((funcptr)func, 2, vs);
}

float dfdx(float x, float y) {
    return df(x, y)[0];
}

float dfdy(float x, float y) {
    return df(x, y)[1];
}

std::vector<float> d2f(float x, float y) {
    float vs[2] = {x, y};
    std::vector<float> ans_dx = gradients((funcptr)dfdx, 2, vs);
    std::vector<float> ans_dy = gradients((funcptr)dfdy, 2, vs);
    
    ans_dx.insert(ans_dx.end(), ans_dy.begin(), ans_dy.end());
    return ans_dx;
}

int main(int argc, const char * argv[]) {
    float fdx = dfdx(3.0f, 4.0f);
    float fdy = dfdy(3.0f, 4.0f);
    
    std::cout << "dfdx: " << fdx << " dfdy: " << fdy << std::endl;
    
    std::vector<float> ds = d2f(3.0f, 4.0f);
    
    std::cout << "get result:" << std::endl;
    for(auto f: ds) {
        std::cout << f << std::endl;
    }
    
    return 0;
}
