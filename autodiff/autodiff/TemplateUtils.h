//
//  TemplateUtils.h
//  autodiff
//
//  Created by AirChen on 2021/3/26.
//

#ifndef TemplateUtils_h
#define TemplateUtils_h

#include <vector>

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

template<typename Type, typename Function, int ...S>
Type call(Function& fp, seq<S...>, Type* v) {
    return fp(v[S]...);
}

template<typename Type, typename Function, typename... TS>
Type call_func(Function& fp, Type* v)
{
    typename gen_seq<sizeof...(TS)+1>::type fo;
    return call<Type>(std::forward<Function&>(fp), fo, v);
}

template <int argc, typename Function, typename Type>
struct CallFuncWrapper{
    constexpr static decltype(call_func<Type, Function, Type, Type, Type>) func_ = call_func<Type, Function, Type, Type, Type>;
};

template <typename Function, typename Type>
struct CallFuncWrapper<1, Function, Type>{
    constexpr static auto func_ = call_func<Type, Function, Type>;
};

template <typename Function, typename Type>
struct CallFuncWrapper<2, Function, Type>{
    constexpr static auto func_ = call_func<Type, Function, Type, Type>;
};

template <typename Type, typename Function, typename... Args>
std::vector<Type> gradients(const Function& fp, const Args&... args) {
    std::vector<Type> ans;
    constexpr auto func = CallFuncWrapper<sizeof...(args), Function, Type>::func_;
    
    Type base_func_value = fp(args...);
    for (int i = 0; i < sizeof...(args); i++) {
        Type params[sizeof...(args)+1] = {0, args...};
        params[i+1] += EPS;
        
        Type tw_value = func(fp, params);
        Type derivative = (tw_value - base_func_value)/EPS;
        ans.push_back(derivative);
    }

    return ans;
}


#endif /* TemplateUtils_h */
