//
//  main.cpp
//  autodiff
//
//  Created by tuRen on 2021/3/3.
//

#include <iostream>
#include <vector>
#include <functional>
#include <tuple>

#define EPS 0.0001

template<int...>
struct seq {};

template<int N, int ...S>
struct gen_seq : gen_seq<N - 1, N - 1, S...> {};

template<int ...S>
struct gen_seq<1, S...>
{
    using type = seq<S...>;
};

template<typename Function, int ...S>
double call(Function& fp, seq<S...>, double* v) {
    return fp(v[S]...);
}

template<typename Function, typename... TS>
double call_func(Function& fp, double* v)
{
    typename gen_seq<sizeof...(TS)+1>::type fo;
    return call(std::forward<Function&>(fp), fo, v);
}

//int func(int x, int y, int z, int a) {
//    std::cout << x << " " << y << " " << z << " " << a << std::endl;
//    return x + y + z + a;
//}
//
//int main()
//{
//    int v[] = { 100,200,300,400,500,600 };
//    int result = call_func<decltype(func), int, int, int, int, int>(func, v);
//
//    std::cout << "result: " << result << std::endl;
//    return 0;
//}

template <typename Function, typename... Args>
std::vector<double> gradients(const Function& fp, const Args&... args) {
    std::vector<double> ans;
    
    double base_func_value = fp(args...);
    for (int i = 0; i < sizeof...(args); i++) {
        double params[sizeof...(args)+1] = {0, args...};
        params[i+1] += EPS;
        
        double tw_value = call_func<decltype(fp), double, double>(fp, params);
        double derivative = (tw_value - base_func_value)/EPS;
        ans.push_back(derivative);
    }

    return ans;
}

double func(double x, double y) {
    return x * x * y + y + 2;
}

std::vector<double> df(double x, double y) {
    return gradients(func, x, y);
}

double dfdx(double x, double y) {
    return df(x, y)[0];
}

double dfdy(double x, double y) {
    return df(x, y)[1];
}

std::vector<double> d2f(double x, double y) {
    auto ans_dx = gradients(dfdx, x, y);
    auto ans_dy = gradients(dfdy, (double)x, (double)y);

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

#include "ToyDatas.hpp"

int fmain(int argc, const char * argv[]) {
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

//double z(double x) {
//    return sin(pow(x, 2));
//}
//
//int zmain(int argc, const char * argv[]) {
//    auto lam = [=](double lx, double o, double e) -> double {
//        return z(3 + lx);
//    };
//
//    std::vector<double> ans = gradients(1, lam);
//
//    std::cout << "test func: dz = " << ans[0] << std::endl;
//
//    return 0;
//}
