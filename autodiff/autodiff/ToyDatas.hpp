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

class Object {
public:
    virtual double evaluate() = 0;
    virtual std::string str() = 0;
    virtual Object* gradient(Object* v) = 0;
};

class Const: public Object {
    double value;
    
public:
    Const(double o): value(o) {}
    
    double evaluate() override { return value; }
    std::string str() override { return std::to_string(value); }
    Object * gradient(Object* v) override { return new Const(0); }
};

class Var: public Object {
    double value;
    std::string name;
    
public:
    Var(std::string k, double v = 0.01): name(k), value(v) {}
    
    void setValue(double v) { value = v; }
    double evaluate() override { return value; }
    std::string str() override { return name; }
    Object * gradient(Object *v) override
    {
        if (v->str() == str()) {
            return new Const(1);
        }
        return new Const(0);
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
    
    double evaluate() override
    {
        return aObject->evaluate() + bObject->evaluate();
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
};

class Mul: public Object, public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    
    double evaluate() override
    {
        return aObject->evaluate() * bObject->evaluate();
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
};

#endif /* ToyDatas_hpp */
