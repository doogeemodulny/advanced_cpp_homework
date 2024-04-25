#include <iostream>
#include <map>
#include <string>
#include <memory>

// Базовый класс выражения
class Expression {
public:
    virtual void print() const = 0;
    virtual int calculate(const std::map<std::string, int>& context) const = 0;
    virtual ~Expression() {};
};

// Класс для оператора сложения
class Addition : public Expression {
private:
    Expression* left;
    Expression* right;

public:
    Addition(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r) : left(&*l), right(&*r) {}
    Addition(Expression* l, std::shared_ptr<Expression> r) : left(l), right(&*r) {}
    Addition(std::shared_ptr<Expression> l, Expression* r) : left(&*l), right(r) {}
    Addition(Expression* l, Expression* r) : left(l), right(r) {}

    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << " + ";
        right->print();
        std::cout << ")";
    }

    int calculate(const std::map<std::string, int>& context) const override {
        return left->calculate(context) + right->calculate(context);
    }

    ~Addition()
    {
        delete left;
        delete right;
    }
};

class Subtraction : public Expression {
private:
    Expression* left;
    Expression* right;

public:
    Subtraction(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r) : left(&*l), right(&*r) {}
    Subtraction(Expression* l, std::shared_ptr<Expression> r) : left(l), right(&*r) {}
    Subtraction(std::shared_ptr<Expression> l, Expression* r) : left(&*l), right(r) {}
    Subtraction(Expression* l, Expression* r) : left(l), right(r) {}

    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << " - ";
        right->print();
        std::cout << ")";
    }

    int calculate(const std::map<std::string, int>& context) const override {
        return left->calculate(context) - right->calculate(context);
    }

    ~Subtraction()
    {
        delete left;
        delete right;
    }
};

class Multiplication : public Expression {
private:
    Expression* left;
    Expression* right;

public:
    Multiplication(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r) : left(&*l), right(&*r) {}
    Multiplication(Expression* l, std::shared_ptr<Expression> r) : left(l), right(&*r) {}
    Multiplication(std::shared_ptr<Expression> l, Expression* r) : left(&*l), right(r) {}
    Multiplication(Expression* l, Expression* r) : left(l), right(r) {}

    void print() const override {
        std::cout << "(";
        left->print();
        std::cout << " * ";
        right->print();
        std::cout << ")";
    }

    int calculate(const std::map<std::string, int>& context) const override {
        return left->calculate(context) * right->calculate(context);
    }

    ~Multiplication()
    {
        delete left;
        delete right;
    }

};

// Класс для константы
class Constant : public Expression {
private:
    int value;

public:
    Constant(int v) : value(v) {}

    void print() const override {
        std::cout << value;
    }

    int calculate(const std::map<std::string, int>& context) const override {
        return value;
    }
};

// Класс для переменной
class Variable : public Expression {
private:
    std::string name;

public:
    Variable(const std::string& n) : name(n) {}

    void print() const override {
        std::cout << name;
    }

    int calculate(const std::map<std::string, int>& context) const override {
        return context.at(name);
    }
};


// Фабрика для создания и удаления объектов переменных и констант
class ExpressionFactory {
public:
    ExpressionFactory()
    {
        for(int i=-5; i<257;i++)
        {
            mConstants[i] = std::make_shared<Constant>(i);
        }
    };
    std::shared_ptr<Constant> createConstant( int value )
    {
        Constants::const_iterator it = mConstants.find( value );
        if ( mConstants.end() == it ) {
            mConstants[value] = std::make_shared<Constant>(value);
            std::cout<<"creating Constant with value: "<<value<<" | new one created\n";
            return mConstants[value] ;
        } else {
            std::cout<<"creating Constant with value: "<<value<<" | already exists\n";
            return it->second;
        }
    }

    std::shared_ptr<Variable> createVariable(const std::string& name) 
    {
        Variables::const_iterator it = mVariables.find( name );
        if ( mVariables.end() == it ) {
             mVariables[name] = std::make_shared<Variable>(name);
             std::cout<<"creating Variable with name: "<<name<<" | new one created\n";
            return mVariables[name] ;
        } else {
            std::cout<<"creating Variable with name: "<<name<<" | already exists\n";
            return it->second;
        }
    }

private:
    using Constants = std::map < int, std::shared_ptr<Constant> >;
    Constants mConstants;

    using Variables = std::map <std::string, std::shared_ptr<Variable>>;
    Variables mVariables;
};

int main() {
    ExpressionFactory factory;
    std::shared_ptr<Constant> c = factory.createConstant(2);
    std::shared_ptr<Variable> v = factory.createVariable("x");
    Addition* expression1 = new Addition(c, v);
    Multiplication* expression2 = new Multiplication(expression1, v);

    std::map<std::string, int> context;
    context["x"] = 3;

    expression1->print();
    std::cout << " = " << expression1->calculate(context) << std::endl;

    expression2->print();
    std::cout << " = " << expression2->calculate(context) << std::endl;

    std::shared_ptr<Constant> c2 = factory.createConstant(2);
    std::shared_ptr<Constant> c3 = factory.createConstant(356);
    std::shared_ptr<Variable> v2 = factory.createVariable("x");
    std::shared_ptr<Variable> v3 = factory.createVariable("y");

    context["y"] = 4;
    Subtraction* expression3 = new Subtraction(v3, expression2);
    expression3->print();
    std::cout << " = " << expression3->calculate(context) << std::endl;


    Addition* expression4 = new Addition(c, c2);
    expression4->print();
    std::cout << " = " << expression4->calculate(context) << std::endl;

    delete expression3;

    return 0;
}
