# autodiff
    A fantastic autodiff tools...

## How to use?

```
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

```

## TODO:

1. Set up Xcode project, implimentation auto gradient functions.    Doing
2. Try to add google test suit.                                     Done
3. Add matrix Supports.                                             Done
4. Switch project to cmake tools manager.
