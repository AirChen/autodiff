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
#include <memory>

typedef double real;

class Object {
public:
    real _gradient = 0;
    real _value = 0;
    
    virtual real evaluate() = 0;
    virtual std::string str() = 0;
    virtual std::shared_ptr<Object> gradient(Object* o) {
        return gradient(std::shared_ptr<Object>(o));
    };
    virtual std::shared_ptr<Object> gradient(std::shared_ptr<Object> o) = 0;
    virtual void backpropagate(real gradient) = 0;
};

class Const: public Object {
public:
    Const(real o) { _value = o; }
    
    real evaluate() override { return _value; }
    std::string str() override { return std::to_string(_value); }
    std::shared_ptr<Object> gradient(std::shared_ptr<Object> o) override { return std::make_shared<Const>(0.0); }
    void backpropagate(real gradient) override {}
    
    static std::shared_ptr<Const> instance(real o) { return std::make_shared<Const>(o); }
};

#define Const(o) Const::instance(o)

class Var: public Object {
    std::string name;
    
public:
    Var(std::string k, real v = 0.01): name(k) { _value = v; }
    
    void setValue(real v) { _value = v; }
    real evaluate() override { return _value; }
    std::string str() override { return name; }
    std::shared_ptr<Object> gradient(std::shared_ptr<Object> o) override
    {
        if (o->str() == str()) {
            return std::make_shared<Const>(1.0);
        }
        return std::make_shared<Const>(0);
    }
    void backpropagate(real gradient) override {
        _gradient += gradient;
    }
    
    static std::shared_ptr<Var> instance(std::string k, real v = 0.01)
    {
        return std::make_shared<Var>(k, v);
    }
};

#define Var(k) Var::instance(k)
//#define Var(k, v) Var::instance(k, v)

class BinaryOperator {
protected:
    std::shared_ptr<Object> aObject;
    std::shared_ptr<Object> bObject;
    
public:
    BinaryOperator(Object* a, Object* b): aObject(std::shared_ptr<Object>(a)), bObject(std::shared_ptr<Object>(b)) {}
    BinaryOperator(std::shared_ptr<Object> a, std::shared_ptr<Object> b): aObject(a), bObject(b) {}
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
    
    std::shared_ptr<Object> gradient(std::shared_ptr<Object> o) override
    {
        auto ad = aObject->gradient(o);
        auto bd = bObject->gradient(o);
        
        return std::make_shared<Add>(ad, bd);
    }
    
    void backpropagate(real gradient) override
    {
        aObject->backpropagate(gradient);
        bObject->backpropagate(gradient);
    }
    
    static std::shared_ptr<Add> instance(std::shared_ptr<Object> a, std::shared_ptr<Object> b)
    {
        return std::make_shared<Add>(a, b);
    }
    
    static std::shared_ptr<Add> instance(Object* a, Object* b)
    {
        return std::make_shared<Add>(a, b);
    }
};

#define Add(a, b) Add::instance((a), (b))

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
    
    std::shared_ptr<Object> gradient(std::shared_ptr<Object> o) override
    {
        auto ad = aObject->gradient(o);
        auto bd = bObject->gradient(o);
        
        auto m0 = std::make_shared<Mul>(ad, bObject);
        auto m1 = std::make_shared<Mul>(aObject, bd);
        
        return std::make_shared<Add>(m0, m1);
    }
    
    void backpropagate(real gradient) override
    {
        aObject->backpropagate(bObject->_value * gradient);
        bObject->backpropagate(aObject->_value * gradient);
    }
    
    static std::shared_ptr<Mul> instance(std::shared_ptr<Object> a, std::shared_ptr<Object> b)
    {
        return std::make_shared<Mul>(a, b);
    }
    
    static std::shared_ptr<Mul> instance(Object* a, Object* b)
    {
        return std::make_shared<Mul>(a, b);
    }
};

#define Mul(a, b) Mul::instance((a), (b))

#endif /* ToyDatas_hpp */
