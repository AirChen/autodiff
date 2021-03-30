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

template<typename T>
class elect{
public:
    T value;
    
    elect(): value(0) {};
    elect(const T&& t): value(t) {};
    elect(const elect<T>& o): value(o.value) {};
    ~elect(){};
    
    virtual std::string description();
    
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
    
    
    elect& operator+=(const elect& other) {
        value += other.value;
        return *this;
    }
    
    elect& operator-=(const elect& other) {
        value -= other.value;
        return *this;
    }
    
    elect& operator*=(const elect& other) {
        value *= other.value;
        return *this;
    }
    
    elect& operator/=(const elect& other) {
//        assert(other.value != 0);
        
        value /= other.value;
        return *this;
    }
};

template<typename T>
class conelect: public elect<T> {
public:
    using elect<T>::elect;
    
    virtual std::string description() override {
        return "( " + std::to_string(this->value) + " )";
    }
};

enum sielect_t {
    sielect_t_sin,
    sielect_t_cos,
    sielect_t_neg
};

template<typename T>
class sielect: public elect<T> {
public:
    elect<T> o;
    sielect_t type;
    
    sielect(elect<T>&& e, sielect_t&& t): o(e), type(t){};
    ~sielect(){};
    
    virtual std::string description() override {
        return "xx " + o.description();
    }
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
    elect<T> left;
    elect<T> right;
    bielect_t type;
        
    bielect(const elect<T>& lhs, const elect<T>& rhs, const bielect_t& t): left(lhs), right(rhs), type(t){
        switch (t) {
            case bielect_t_add:
                this->value = lhs.value + rhs.value;
                break;
            case bielect_t_sub:
                this->value = lhs.value - rhs.value;
                break;
            case bielect_t_mul:
                this->value = lhs.value * rhs.value;
                break;
            case bielect_t_div:
                // rhs.value != 0
                this->value = lhs.value + rhs.value;
                break;
                
            default:
                // error
                break;
        }
    };
    ~bielect(){};
    
    std::string description() override {
        std::string ans;
        switch (type) {
            case bielect_t_add:
                ans = left.description() + " + " + right.description();
                break;
            case bielect_t_sub:
                ans = left.description() + " - " + right.description();
                break;
            case bielect_t_mul:
                ans = left.description() + " * " + right.description();
                break;
            case bielect_t_div:
                ans = left.description() + " / " + right.description();
                break;
                
            default:
                break;
        }
        return ans;
    }
};

template<typename T>
std::string elect<T>::description() {
    std::string ans;
    if (dynamic_cast<bielect<T>*>(this) != nullptr) {
        auto bi = dynamic_cast<bielect<T>*>(this);
        ans = bi->description();
    } else if (dynamic_cast<sielect<T>*>(this) != nullptr) {
        auto si = dynamic_cast<sielect<T>*>(this);
        ans = si->description();
    } else if (dynamic_cast<conelect<T>*>(this) != nullptr) {
        auto con = dynamic_cast<conelect<T>*>(this);
        ans = con->description();
    } else {
        ans = std::to_string(value);
    }
    return ans;
}

namespace fake {

    template<typename T>
    elect<T> operator+(const elect<T>& lhs, const elect<T>& rhs)
    {
        bielect<T> bi(lhs, rhs, bielect_t_add);
        return bi;
    }

    template<typename T>
    elect<T> operator+(T lhs, const elect<T>& rhs)
    {
        conelect<T> c(lhs);
        bielect<T> bi(c, rhs, bielect_t_add);
        return bi;
    }

    template<typename T>
    elect<T> operator+(const elect<T>& lhs, T rhs)
    {
        conelect<T> c(rhs);
        bielect<T> bi(lhs, c, bielect_t_add);
        return bi;
    }

    template<typename T>
    elect<T> operator-(const elect<T>& lhs, const elect<T>& rhs)
    {
        bielect<T> bi(lhs, rhs, bielect_t_sub);
        return bi;
    }

    template<typename T>
    elect<T> operator-(T lhs, const elect<T>& rhs)
    {
        conelect<T> c(lhs);
        bielect<T> bi(c, rhs, bielect_t_sub);
        return bi;
    }

    template<typename T>
    elect<T> operator-(const elect<T>& lhs, T rhs)
    {
        conelect<T> c(rhs);
        bielect<T> bi(lhs, c, bielect_t_sub);
        return bi;
    }

    template<typename T>
    elect<T> operator*(const elect<T>& lhs, const elect<T>& rhs)
    {
        bielect<T> bi(lhs, rhs, bielect_t_mul);
        return bi;
    }

    template<typename T>
    elect<T> operator*(T lhs, const elect<T>& rhs)
    {
        conelect<T> c;
        c.value = lhs;// why ?
        bielect<T> bi(c, rhs, bielect_t_mul);
        return bi;
    }

    template<typename T>
    elect<T> operator*(const elect<T>& lhs, T rhs)
    {
        conelect<T> c(rhs);
        bielect<T> bi(lhs, c, bielect_t_mul);
        return bi;
    }

    template<typename T>
    elect<T> operator/(const elect<T>& lhs, const elect<T>& rhs)
    {
        bielect<T> bi(lhs, rhs, bielect_t_div);
        return bi;
    }

    template<typename T>
    elect<T> operator/(T lhs, const elect<T>& rhs)
    {
        conelect<T> c(lhs);
        bielect<T> bi(c, rhs, bielect_t_div);
        return bi;
    }

    template<typename T>
    elect<T> operator/(const elect<T>& lhs, T rhs)
    {
        conelect<T> c(rhs);
        bielect<T> bi(lhs, c, bielect_t_div);
        return bi;
    }

}

#endif /* forward_hpp */
