//
//  ToyDatas.hpp
//  autodiff
//
//  Created by AirChen on 2021/3/4.
//

#ifndef ToyDatas_hpp
#define ToyDatas_hpp

#include <stdio.h>
#include <string>

typedef double real;

class Object {
public:
    real _gradient = 0;
    real _value = 0;
    
    virtual real evaluate() = 0;
    virtual std::string str() = 0;
    virtual Object* gradient(Object* v) = 0;
    virtual void backpropagate(real gradient) = 0;
};

class Const: public Object {
public:
    Const(real o) { _value = o; }
    
    real evaluate() override { return _value; }
    std::string str() override { return std::to_string(_value); }
    Object * gradient(Object* v) override { return new Const(0); }
    void backpropagate(real gradient) override {}
};

class Var: public Object {
    std::string name;
    
public:
    Var(std::string k, real v = 0.01): name(k) { _value = v; }
    
    void setValue(real v) { _value = v; }
    real evaluate() override { return _value; }
    std::string str() override { return name; }
    Object * gradient(Object *v) override
    {
        if (v->str() == str()) {
            return new Const(1);
        }
        return new Const(0);
    }
    void backpropagate(real gradient) override {
        _gradient += gradient;
    }
};

class BinaryOperator {
protected:
    Object *aObject;
    Object *bObject;
    
public:
    BinaryOperator(Object* a, Object* b): aObject(a), bObject(b) {}
};

class Add: public Object, public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    
    real evaluate() override
    {
        _value = aObject->evaluate() + bObject->evaluate();
        return _value;
    }
    
    std::string str() override
    {
        return aObject->str() + " + " + bObject->str();
    }
    
    Object * gradient(Object *v) override
    {
        Object* ad = aObject->gradient(v);
        Object* bd = bObject->gradient(v);
        
        return new Add(ad, bd);
    }
    
    void backpropagate(real gradient) override
    {
        aObject->backpropagate(gradient);
        bObject->backpropagate(gradient);
    }
};

class Mul: public Object, public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    
    real evaluate() override
    {
        _value = aObject->evaluate() * bObject->evaluate();
        return _value;
    }
    
    std::string str() override
    {
        return "(" + aObject->str() + ") * (" + bObject->str() + ")";
    }
    
    Object * gradient(Object *v) override
    {
        Object* ad = aObject->gradient(v);
        Object* bd = bObject->gradient(v);
        
        Object* m0 = new Mul(ad, bObject);
        Object* m1 = new Mul(aObject, bd);
        
        return new Add(m0, m1);
    }
    
    void backpropagate(real gradient) override
    {
        aObject->backpropagate(bObject->_value * gradient);
        bObject->backpropagate(aObject->_value * gradient);
    }
};

#endif /* ToyDatas_hpp */
