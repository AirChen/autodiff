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
#include <vector>

enum elect_type {
    elect_type_atomic,
    elect_type_var,
    elect_type_const,
    elect_type_single,
    elect_type_binary
};

enum single_type {
    single_type_sin,
    single_type_cos,
    single_type_neg
};

enum binary_type {
    binary_type_add,
    binary_type_sub,
    binary_type_mul,
    binary_type_div
};


template<typename T>
struct elect{
    T value;
    elect_type type;
    char name;
    int subType;
    std::vector<elect<T>> nodes;
        
    elect(): value(0), type(elect_type_atomic), name(0) {};
    elect(const T&& t): value(t), type(elect_type_const), name(0) {}; // to const
    ~elect(){};
    
    std::string description();
    elect gradient(const elect& e);
    
    // https://stackoverflow.com/questions/5695548/public-friend-swap-member-function
    friend void swap(elect& fhs, const elect& lhs) {
        elect& v = const_cast<elect&>(lhs);
        
        using std::swap;
        swap(fhs.value, v.value);
        swap(fhs.type, v.type);
        swap(fhs.subType, v.subType);
        swap(fhs.nodes, v.nodes);
        // not swap name for var
    }
    
    elect& operator=(const elect& other) {
        swap(*this, other);
        return *this;
    }
    
    elect& operator+=(const elect& other) {
        elect<T> o = *this + other;
        swap(*this, o);
        return *this;
    }
    
    elect& operator-=(const elect& other) {
        elect<T> o = *this - other;
        swap(*this, o);
        return *this;
    }
    
    elect& operator*=(const elect& other) {
        elect<T> o = *this * other;
        swap(*this, o);
        return *this;
    }
    
    elect& operator/=(const elect& other) {
//        assert(other.value != 0);
        elect<T> o = *this / other;
        swap(*this, o);
        return *this;
    }
};

template<typename T>
struct conelect: public elect<T> {
    conelect(const T&& t) {
        this->value = t;
        this->type = elect_type_const;
    };
    ~conelect() {};
};

template<typename T>
struct varelect: public elect<T> {
    varelect(T v, char c) {
        this->value = v;
        this->type = elect_type_var;
        this->name = c;
    };
    varelect(const elect<T>& o) {
        swap(*this, o);
    }
    ~varelect() {};
    
    varelect& operator=(T v) {
        this->value = v;
        this->type = elect_type_var;
        this->nodes.clear();
        return *this;
    }
};

template<typename T>
struct sielect: public elect<T> {
    sielect(elect<T>& e, single_type tp) {
//        this->value = e.value;            TODO:
        this->type = elect_type_single;
        this->subType = tp;
        this->nodes.push_back(e);
    };
    ~sielect() {};
};

template<typename T>
class bielect: public elect<T> {
public:
    bielect(const elect<T>& lhs, const elect<T>& rhs, binary_type tp) {
        switch (tp) {
            case binary_type_add:
                this->value = lhs.value + rhs.value;
                break;
            case binary_type_sub:
                this->value = lhs.value - rhs.value;
                break;
            case binary_type_mul:
                this->value = lhs.value * rhs.value;
                break;
            case binary_type_div:
                // rhs.value != 0
                this->value = lhs.value / rhs.value;
                break;

            default:
                // error
                break;
        }
        
        this->type = elect_type_binary;
        this->subType = tp;
        
        this->nodes.push_back(lhs);
        this->nodes.push_back(rhs);
    };
    ~bielect(){};
};

template<typename T>
std::string elect<T>::description() {
    if (type == elect_type_atomic) {
        return "atomic type!";
    } else if (type == elect_type_var) {
        std::string key;
        key.push_back((char)name);
        return "var(" + key + " : " + std::to_string(value) + ")";
    } else if (type == elect_type_const) {
        return "( " + std::to_string(this->value) + " )";
    } else if (type == elect_type_single) {
        return "xx " + nodes[0].description();
    } else if (type == elect_type_binary) {
        switch (subType) {
            case binary_type_add:
                return  nodes[0].description() + " + " + nodes[1].description();
            case binary_type_sub:
                return  nodes[0].description() + " - " + nodes[1].description();
            case binary_type_mul:
                return  nodes[0].description() + " * " + nodes[1].description();
            case binary_type_div:
                return  nodes[0].description() + " / " + nodes[1].description();

            default:
                return "binary func not define!";
        }
    }
    
    return "not define elect type!";
};
 
template<typename T>
elect<T> operator+(const elect<T>& lhs, const elect<T>& rhs)
{
    bielect<T> bi(lhs, rhs, binary_type_add);
    return bi;
}

template<typename T>
elect<T> operator+(T lhs, const elect<T>& rhs)
{
    conelect<T> c(lhs);
    bielect<T> bi(c, rhs, binary_type_add);
    return bi;
}

template<typename T>
elect<T> operator+(int lhs, const elect<T>& rhs)
{
    conelect<T> c(static_cast<T>(lhs));
    bielect<T> bi(c, rhs, binary_type_add);
    return bi;
}

template<typename T>
elect<T> operator+(const elect<T>& lhs, T rhs)
{
    conelect<T> c(rhs);
    bielect<T> bi(lhs, c, binary_type_add);
    return bi;
}

template<typename T>
elect<T> operator+(const elect<T>& lhs, int rhs)
{
    conelect<T> c(static_cast<T>(rhs));
    bielect<T> bi(lhs, c, binary_type_add);
    return bi;
}

template<typename T>
elect<T> operator-(const elect<T>& lhs, const elect<T>& rhs)
{
    bielect<T> bi(lhs, rhs, binary_type_sub);
    return bi;
}

template<typename T>
elect<T> operator-(T lhs, const elect<T>& rhs)
{
    conelect<T> c(lhs);
    bielect<T> bi(c, rhs, binary_type_sub);
    return bi;
}

template<typename T>
elect<T> operator-(int lhs, const elect<T>& rhs)
{
    conelect<T> c(static_cast<T>(lhs));
    bielect<T> bi(c, rhs, binary_type_sub);
    return bi;
}

template<typename T>
elect<T> operator-(const elect<T>& lhs, T rhs)
{
    conelect<T> c(rhs);
    bielect<T> bi(lhs, c, binary_type_sub);
    return bi;
}

template<typename T>
elect<T> operator-(const elect<T>& lhs, int rhs)
{
    conelect<T> c(static_cast<T>(rhs));
    bielect<T> bi(lhs, c, binary_type_sub);
    return bi;
}

template<typename T>
elect<T> operator*(const elect<T>& lhs, const elect<T>& rhs)
{
    bielect<T> bi(lhs, rhs, binary_type_mul);
    return bi;
}

template<typename T>
elect<T> operator*(T lhs, const elect<T>& rhs)
{
    conelect<T> c(lhs);
    bielect<T> bi(c, rhs, binary_type_mul);
    return bi;
}

template<typename T>
elect<T> operator*(int lhs, const elect<T>& rhs)
{
    conelect<T> c(static_cast<T>(lhs));
    bielect<T> bi(c, rhs, binary_type_mul);
    return bi;
}

template<typename T>
elect<T> operator*(const elect<T>& lhs, T rhs)
{
    conelect<T> c(rhs);
    bielect<T> bi(lhs, c, binary_type_mul);
    return bi;
}

template<typename T>
elect<T> operator*(const elect<T>& lhs, int rhs)
{
    conelect<T> c(static_cast<T>(rhs));
    bielect<T> bi(lhs, c, binary_type_mul);
    return bi;
}

template<typename T>
elect<T> operator/(const elect<T>& lhs, const elect<T>& rhs)
{
    bielect<T> bi(lhs, rhs, binary_type_div);
    return bi;
}

template<typename T>
elect<T> operator/(T lhs, const elect<T>& rhs)
{
    conelect<T> c(lhs);
    bielect<T> bi(c, rhs, binary_type_div);
    return bi;
}

template<typename T>
elect<T> operator/(int lhs, const elect<T>& rhs)
{
    conelect<T> c(static_cast<T>(lhs));
    bielect<T> bi(c, rhs, binary_type_div);
    return bi;
}

template<typename T>
elect<T> operator/(const elect<T>& lhs, T rhs)
{
    conelect<T> c(rhs);
    bielect<T> bi(lhs, c, binary_type_div);
    return bi;
}

template<typename T>
elect<T> operator/(const elect<T>& lhs, int rhs)
{
    conelect<T> c(static_cast<T>(rhs));
    bielect<T> bi(lhs, c, binary_type_div);
    return bi;
}

namespace compare {
    template<typename T>
    bool operator==(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value == rhs.value;
    }

    template<typename T>
    bool operator==(const elect<T>& lhs, T rhs) {
        return lhs.value == rhs;
    }

    template<typename T>
    bool operator==(const elect<T>& lhs, int rhs) {
        return lhs.value == rhs;
    }

    template<typename T>
    bool operator==(T lhs, const elect<T>& rhs) {
        return lhs == rhs.value;
    }

    template<typename T>
    bool operator==(int lhs, const elect<T>& rhs) {
        return lhs == rhs.value;
    }

    template<typename T>
    bool operator!=(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value != rhs.value;
    }

    template<typename T>
    bool operator!=(const elect<T>& lhs, T rhs) {
        return lhs.value != rhs;
    }

    template<typename T>
    bool operator!=(const elect<T>& lhs, int rhs) {
        return lhs.value != rhs;
    }

    template<typename T>
    bool operator!=(T lhs, const elect<T>& rhs) {
        return lhs != rhs.value;
    }

    template<typename T>
    bool operator!=(int lhs, const elect<T>& rhs) {
        return lhs != rhs.value;
    }


    // >
    template<typename T>
    bool operator>(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value > rhs.value;
    }

    template<typename T>
    bool operator>(const elect<T>& lhs, T rhs) {
        return lhs.value > rhs;
    }

    template<typename T>
    bool operator>(const elect<T>& lhs, int rhs) {
        return lhs.value > rhs;
    }

    template<typename T>
    bool operator>(T lhs, const elect<T>& rhs) {
        return lhs > rhs.value;
    }

    template<typename T>
    bool operator>(int lhs, const elect<T>& rhs) {
        return lhs > rhs.value;
    }

    // >=
    template<typename T>
    bool operator>=(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value >= rhs.value;
    }

    template<typename T>
    bool operator>=(const elect<T>& lhs, T rhs) {
        return lhs.value >= rhs;
    }

    template<typename T>
    bool operator>=(const elect<T>& lhs, int rhs) {
        return lhs.value >= rhs;
    }

    template<typename T>
    bool operator>=(T lhs, const elect<T>& rhs) {
        return lhs >= rhs.value;
    }

    template<typename T>
    bool operator>=(int lhs, const elect<T>& rhs) {
        return lhs >= rhs.value;
    }

    // <
    template<typename T>
    bool operator<(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value < rhs.value;
    }

    template<typename T>
    bool operator<(const elect<T>& lhs, T rhs) {
        return lhs.value < rhs;
    }

    template<typename T>
    bool operator<(const elect<T>& lhs, int rhs) {
        return lhs.value < rhs;
    }

    template<typename T>
    bool operator<(T lhs, const elect<T>& rhs) {
        return lhs < rhs.value;
    }

    template<typename T>
    bool operator<(int lhs, const elect<T>& rhs) {
        return lhs < rhs.value;
    }

    // <=
    template<typename T>
    bool operator<=(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value <= rhs.value;
    }

    template<typename T>
    bool operator<=(const elect<T>& lhs, T rhs) {
        return lhs.value <= rhs;
    }

    template<typename T>
    bool operator<=(const elect<T>& lhs, int rhs) {
        return lhs.value <= rhs;
    }

    template<typename T>
    bool operator<=(T lhs, const elect<T>& rhs) {
        return lhs <= rhs.value;
    }

    template<typename T>
    bool operator<=(int lhs, const elect<T>& rhs) {
        return lhs <= rhs.value;
    }
}

template<typename T>
elect<T> elect<T>::gradient(const elect<T>& e) {
    switch (type) {
        case elect_type_var:
        {
            if (name == e.name) {
                return conelect<T>(static_cast<T>(1));
            }
            return conelect<T>(static_cast<T>(0));
        }
            break;
        case elect_type_atomic:
        case elect_type_const:
        {
            return conelect<T>(static_cast<T>(0));
        }
            break;
        case elect_type_single:
        {
            
        }
            break;
        case elect_type_binary:
        {
            elect<T> ad = nodes[0].gradient(e);
            elect<T> bd = nodes[1].gradient(e);
            switch (subType) {
                case binary_type_add:
                {
                    return ad + bd;
                }
                    break;
                case binary_type_sub:
                {
                    return ad - bd;
                }
                    break;
                case binary_type_mul:
                {
                    return ad * nodes[1] + nodes[0] * bd;
                }
                    break;
                case binary_type_div:
                {
//                    (u/v)'=(u'v-uv')/v²
                    return ((ad * nodes[1]) - (nodes[0] * bd)) / (nodes[1] * nodes[1]);
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    
    return conelect<T>(static_cast<T>(0));
}


#endif /* forward_hpp */
