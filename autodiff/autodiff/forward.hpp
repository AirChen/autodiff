//
//  forward.hpp
//  autodiff
//
//  Created by AirChen on 2021/3/28.
//

#ifndef forward_hpp
#define forward_hpp

#include <stdio.h>

template<typename T>
struct elect{
    T value;
    
    elect(T&& t): value(t) {};
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

#endif /* forward_hpp */
