//
//  forward.hpp
//  autodiff
//
//  Created by AirChen on 2021/3/28.
//

#ifndef forward_hpp
#define forward_hpp

#include <stdio.h>
#include <string>

enum elect_type {
    elect_type_ato,
    elect_type_con,
    elect_type_si,
    elect_type_bi
};

template<typename T>
class elect{
public:
    elect_type tp;
    T value;
    
    elect(): value(0.0), tp(elect_type_ato) {};
    elect(const T&& t): value(t), tp(elect_type_ato) {};
    elect(const elect<T>& o): value(o.value), tp(elect_type_ato) {};
    ~elect(){};
    
    bool operator==(const elect&& other) {
        return value == other.value;
    }
    
    bool operator!=(const elect&& other) {
        return value != other.value;
    }
    
    elect& operator+=(const elect&& other) {
        value += other.value;
        return *this;
    }
    
    elect& operator-=(const elect&& other) {
        value -= other.value;
        return *this;
    }
    
    elect& operator*=(const elect&& other) {
        value *= other.value;
        return *this;
    }
    
    elect& operator/=(const elect&& other) {
//        assert(other.value != 0);
        
        value /= other.value;
        return *this;
    }
};

template<typename T>
class conelect: public elect<T> {
public:
    conelect(const T t) {
        this->value = t;
        this->tp = elect_type_con;
    };
    conelect(const conelect<T>& o) {
        this->value = o.value;
        this->tp = elect_type_con;
    };
    ~conelect(){};
};

enum sielect_t {
    sielect_t_sin,
    sielect_t_cos,
    sielect_t_neg
};

template<typename T>
class sielect: public elect<T> {
public:
    std::shared_ptr<elect<T>> o;
    sielect_t type;
    
    sielect(std::shared_ptr<elect<T>> e, sielect_t&& t): o(e), type(t){
        this->tp = elect_type_si;
    };
    ~sielect(){};
};

enum bielect_t {
    bielect_t_add,
    bielect_t_sub,
    bielect_t_mul,
    bielect_t_div
};

template<typename T>
class bielect: public elect<T> {
public:
    std::shared_ptr<elect<T>> left;
    std::shared_ptr<elect<T>> right;
    bielect_t type;
        
    bielect(std::shared_ptr<elect<T>> lhs, std::shared_ptr<elect<T>> rhs, const bielect_t t): left(lhs), right(rhs), type(t){
        switch (t) {
            case bielect_t_add:
                this->value = lhs->value + rhs->value;
                break;
            case bielect_t_sub:
                this->value = lhs->value - rhs->value;
                break;
            case bielect_t_mul:
                this->value = lhs->value * rhs->value;
                break;
            case bielect_t_div:
                // rhs.value != 0
                this->value = lhs->value + rhs->value;
                break;
                
            default:
                // error
                break;
        }
        this->tp = elect_type_bi;
    };
    ~bielect(){};
};

namespace fake {
    template<typename T>
    std::shared_ptr<elect<T>> operator+(std::shared_ptr<elect<T>>& lhs, std::shared_ptr<elect<T>>& rhs)
    {
        return std::make_shared<elect<T>>(lhs, rhs, bielect_t_add);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator+(T lhs, std::shared_ptr<elect<T>> rhs)
    {
        std::shared_ptr<conelect<T>> c = std::make_shared<conelect<T>>(lhs);
        return std::make_shared<elect<T>>(c, rhs, bielect_t_add);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator+(std::shared_ptr<conelect<T>> lhs, T rhs)
    {
        std::shared_ptr<conelect<T>> c = std::make_shared<conelect<T>>(rhs);
        return std::make_shared<elect<T>>(lhs, c, bielect_t_add);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator-(std::shared_ptr<conelect<T>>& lhs, std::shared_ptr<conelect<T>>& rhs)
    {
        return std::make_shared<elect<T>>(lhs, rhs, bielect_t_sub);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator-(T lhs, std::shared_ptr<elect<T>> rhs)
    {
        std::shared_ptr<conelect<T>> c = std::make_shared<conelect<T>>(lhs);
        return std::make_shared<elect<T>>(c, rhs, bielect_t_sub);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator-(std::shared_ptr<conelect<T>> lhs, T rhs)
    {
        std::shared_ptr<conelect<T>> c = std::make_shared<conelect<T>>(rhs);
        return std::make_shared<elect<T>>(lhs, c, bielect_t_sub);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator*(std::shared_ptr<conelect<T>>& lhs, std::shared_ptr<conelect<T>>& rhs)
    {
        return std::make_shared<elect<T>>(lhs, rhs, bielect_t_mul);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator*(T lhs, std::shared_ptr<elect<T>> rhs)
    {
        std::shared_ptr<conelect<T>> c = std::make_shared<conelect<T>>(lhs);
        return std::make_shared<elect<T>>(c, rhs, bielect_t_mul);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator*(std::shared_ptr<conelect<T>> lhs, T rhs)
    {
        std::shared_ptr<conelect<T>> c = std::make_shared<conelect<T>>(rhs);
        return std::make_shared<elect<T>>(lhs, c, bielect_t_mul);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator/(std::shared_ptr<conelect<T>>& lhs, std::shared_ptr<conelect<T>>& rhs)
    {
        return std::make_shared<elect<T>>(lhs, rhs, bielect_t_div);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator/(T lhs, std::shared_ptr<elect<T>> rhs)
    {
        std::shared_ptr<conelect<T>> c = std::make_shared<conelect<T>>(lhs);
        return std::make_shared<elect<T>>(c, rhs, bielect_t_div);
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator/(std::shared_ptr<conelect<T>> lhs, T rhs)
    {
        std::shared_ptr<conelect<T>> c = std::make_shared<conelect<T>>(rhs);
        return std::make_shared<elect<T>>(lhs, c, bielect_t_div);
    }

    template<typename T, typename O>
    bool operator==(std::shared_ptr<elect<T>> lhs, O rhs) {
        return lhs->value == rhs;
    }

    template<typename T, typename O>
    bool operator!=(std::shared_ptr<elect<T>> lhs, O rhs) {
        return lhs->value != rhs;
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator+=(std::shared_ptr<elect<T>> o, std::shared_ptr<elect<T>> other) {
        o->value += other->value;
        return o;
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator-=(std::shared_ptr<elect<T>> o, std::shared_ptr<elect<T>> other) {
        o->value -= other->value;
        return o;
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator*=(std::shared_ptr<elect<T>> o, std::shared_ptr<elect<T>> other) {
        o->value *= other->value;
        return o;
    }

    template<typename T>
    std::shared_ptr<elect<T>> operator/=(std::shared_ptr<elect<T>> o, std::shared_ptr<elect<T>> other) {
    //        assert(other.value != 0);
        
        o->value /= other->value;
        return o;
    }

    template<typename T, typename O>
    std::shared_ptr<elect<T>> operator+=(std::shared_ptr<elect<T>> o, O v) {
        o->value += v;
        return o;
    }

    template<typename T, typename O>
    std::shared_ptr<elect<T>> operator-=(std::shared_ptr<elect<T>> o, O v) {
        o->value -= v;
        return o;
    }

    template<typename T, typename O>
    std::shared_ptr<elect<T>> operator*=(std::shared_ptr<elect<T>> o, O v) {
        o->value *= v;
        return o;
    }

    template<typename T, typename O>
    std::shared_ptr<elect<T>> operator/=(std::shared_ptr<elect<T>> o, O v) {
    //        assert(other.value != 0);
        
        o->value /= v;
        return o;
    }
}

#endif /* forward_hpp */
