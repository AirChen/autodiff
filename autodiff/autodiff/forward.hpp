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
    int subType;
    std::vector<elect<T>> nodes;
    
    elect(): value(0), type(elect_type_atomic) {};
    elect(const T&& t): value(t), type(elect_type_atomic) {};
    ~elect(){};
    
    elect& operator+=(const elect&& other) {
        value += other.value;
        type = other.type;
        subType = other.subType;
        nodes = other.nodes;
        return *this;
    }
    
    elect& operator-=(const elect&& other) {
        value -= other.value;
        type = other.type;
        subType = other.subType;
        nodes = other.nodes;
        return *this;
    }
    
    elect& operator*=(const elect&& other) {
        value *= other.value;
        type = other.type;
        subType = other.subType;
        nodes = other.nodes;
        return *this;
    }
    
    elect& operator/=(const elect&& other) {
//        assert(other.value != 0);
        
        value /= other.value;
        type = other.type;
        subType = other.subType;
        nodes = other.nodes;
        return *this;
    }
    
    elect& operator=(const elect& other) {
        value = other.value;
        type = other.type;
        subType = other.subType;
        nodes = other.nodes;
        return *this;
    }
    
    std::string description();
    elect gradient(const elect& e);
};

template<typename T>
struct conelect: public elect<T> {
    conelect(T v) {
        this->value = v;
        this->type = elect_type_const;
    };
    ~conelect() {};
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
        return "var(" + std::to_string(value) + ")";
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
 
namespace fake {
    template<typename T>
    elect<T> operator+(const elect<T>& lhs, const elect<T>& rhs)
    {
        bielect<T> bi(lhs, rhs, binary_type_add);
        return bi;
    }

    template<typename T, typename O>
    elect<T> operator+(O lhs, const elect<T>& rhs)
    {
        conelect<T> c(lhs);
        bielect<T> bi(c, rhs, binary_type_add);
        return bi;
    }

    template<typename T, typename O>
    elect<T> operator+(const elect<T>& lhs, O rhs)
    {
        conelect<T> c(rhs);
        bielect<T> bi(lhs, c, binary_type_add);
        return bi;
    }

    template<typename T>
    elect<T> operator-(const elect<T>& lhs, const elect<T>& rhs)
    {
        bielect<T> bi(lhs, rhs, binary_type_sub);
        return bi;
    }

    template<typename T, typename O>
    elect<T> operator-(O lhs, const elect<T>& rhs)
    {
        conelect<T> c(lhs);
        bielect<T> bi(c, rhs, binary_type_sub);
        return bi;
    }

    template<typename T, typename O>
    elect<T> operator-(const elect<T>& lhs, O rhs)
    {
        conelect<T> c(rhs);
        bielect<T> bi(lhs, c, binary_type_sub);
        return bi;
    }

    template<typename T>
    elect<T> operator*(const elect<T>& lhs, const elect<T>& rhs)
    {
        bielect<T> bi(lhs, rhs, binary_type_mul);
        return bi;
    }

    template<typename T, typename O>
    elect<T> operator*(O lhs, const elect<T>& rhs)
    {
        conelect<T> c(lhs);
        bielect<T> bi(c, rhs, binary_type_mul);
        return bi;
    }

    template<typename T, typename O>
    elect<T> operator*(const elect<T>& lhs, O rhs)
    {
        conelect<T> c(rhs);
        bielect<T> bi(lhs, c, binary_type_mul);
        return bi;
    }

    template<typename T>
    elect<T> operator/(const elect<T>& lhs, const elect<T>& rhs)
    {
        bielect<T> bi(lhs, rhs, binary_type_div);
        return bi;
    }

    template<typename T, typename O>
    elect<T> operator/(O lhs, const elect<T>& rhs)
    {
        conelect<T> c(lhs);
        bielect<T> bi(c, rhs, binary_type_div);
        return bi;
    }

    template<typename T, typename O>
    elect<T> operator/(const elect<T>& lhs, O rhs)
    {
        conelect<T> c(rhs);
        bielect<T> bi(lhs, c, binary_type_div);
        return bi;
    }
}

namespace compare {
    template<typename T>
    bool operator==(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value == rhs.value;
    }

    template<typename T, typename O>
    bool operator==(const elect<T>& lhs, const O rhs) {
        return lhs.value == rhs;
    }

    template<typename T, typename O>
    bool operator==(const O lhs, const elect<T>& rhs) {
        return lhs == rhs.value;
    }

    template<typename T>
    bool operator!=(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value != rhs.value;
    }

    template<typename T, typename O>
    bool operator!=(const elect<T>& lhs, const O rhs) {
        return lhs.value != rhs;
    }

    template<typename T, typename O>
    bool operator!=(const O lhs, const elect<T>& rhs) {
        return lhs != rhs.value;
    }


    // >
    template<typename T>
    bool operator>(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value > rhs.value;
    }

    template<typename T, typename O>
    bool operator>(const elect<T>& lhs, const O rhs) {
        return lhs.value > rhs;
    }

    template<typename T, typename O>
    bool operator>(const O lhs, const elect<T>& rhs) {
        return lhs > rhs.value;
    }

    // >=
    template<typename T>
    bool operator>=(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value >= rhs.value;
    }

    template<typename T, typename O>
    bool operator>=(const elect<T>& lhs, const O rhs) {
        return lhs.value >= rhs;
    }

    template<typename T, typename O>
    bool operator>=(const O lhs, const elect<T>& rhs) {
        return lhs >= rhs.value;
    }

    // <
    template<typename T>
    bool operator<(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value < rhs.value;
    }

    template<typename T, typename O>
    bool operator<(const elect<T>& lhs, const O rhs) {
        return lhs.value < rhs;
    }

    template<typename T, typename O>
    bool operator<(const O lhs, const elect<T>& rhs) {
        return lhs < rhs.value;
    }

    // <=
    template<typename T>
    bool operator<=(const elect<T>& lhs, const elect<T>& rhs) {
        return lhs.value <= rhs.value;
    }

    template<typename T, typename O>
    bool operator<=(const elect<T>& lhs, const O rhs) {
        return lhs.value <= rhs;
    }

    template<typename T, typename O>
    bool operator<=(const O lhs, const elect<T>& rhs) {
        return lhs <= rhs.value;
    }
}

using namespace fake;
template<typename T>
elect<T> elect<T>::gradient(const elect<T>& e) {
    switch (type) {
        case elect_type_atomic:
        {
            conelect<T> con(1);
            return con;
            // TODO: ==
//            if (o->str() == str()) {
//                return std::make_shared<Const>(1.0);
//            }
//            return std::make_shared<Const>(0);
        }
            break;
        case elect_type_const:
        {
            conelect<T> con(0);
            return con;
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
    
    return 0; // to be atomic!!
}


#endif /* forward_hpp */
