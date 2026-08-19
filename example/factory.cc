#include <iostream>
#include <memory>

class Fruit
{
public:
    virtual void name() = 0;
};

class Apple : public Fruit
{
public:
    void name() override
    {
        std::cout << "I am an apple " << std::endl;
    }
};

class Banana : public Fruit
{
public:
    void name() override
    {
        std::cout << "I am a banana " << std::endl;
    }
};

class FruitFactory
{
    public:
    static std::shared_ptr<Fruit> create(const std::string &name){
        if(name == "苹果")
        {
            return std::make_shared<Apple>();
        }
        else
        {
            return std::make_shared<Banana>();
        }
    }
};

int main()
{
    std::shared_ptr<Fruit> fruit = FruitFactory::create("苹果");
    fruit->name();

    return 0;
}