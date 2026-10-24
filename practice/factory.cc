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

// class FruitFactory
// {
//     public:
//     static std::shared_ptr<Fruit> create(const std::string &name){
//         if(name == "苹果")
//         {
//             return std::make_shared<Apple>();
//         }
//         else
//         {
//             return std::make_shared<Banana>();
//         }
//     }
// };

// class FruitFactory
// {
// public:
//     virtual std::shared_ptr<Fruit> create() = 0;
// };

// class AppleFacctory : public FruitFactory
// {
// public:
//     std::shared_ptr<Fruit> create()
//     {
//         return std::make_shared<Apple>();
//     }
// };

// class BananaFacctory : public FruitFactory
// {
// public:
//     std::shared_ptr<Fruit> create()
//     {
//         return std::make_shared<Banana>();
//     }
// };

class Animal
{
public:
    virtual void name() = 0;
};

class Lamp : public Animal
{
public:
    void name() override
    {
        std::cout << "I am a lamp" << std::endl;
    }
};

class Dog : public Animal
{
public:
    void name() override
    {
        std::cout << "I am a dog" << std::endl;
    }
};

class Factory
{
public:
    virtual std::shared_ptr<Fruit> getFruit(const std::string &name) = 0;
    virtual std::shared_ptr<Animal> getAnimal(const std::string &name) = 0;
};

class FruitFactory : public Factory
{
public:
    std::shared_ptr<Animal> getAnimal(const std::string &name)
    {
        return std::shared_ptr<Animal>();
    }
    std::shared_ptr<Fruit> getFruit(const std::string &name)
    {
        if (name == "苹果")
        {
            return std::make_shared<Apple>();
        }
        else
        {
            return std::make_shared<Banana>();
        }
    }
};

class AnimalFactory : public Factory
{
public:
    std::shared_ptr<Animal> getAnimal(const std::string &name)
    {
        if (name == "小狗")
        {
            return std::make_shared<Dog>();
        }
        else
        {
            return std::make_shared<Lamp>();
        }
    }
    std::shared_ptr<Fruit> getFruit(const std::string &name)
    {
        return std::shared_ptr<Fruit>();
    }
};

class FactoryProducer
{
public:
    static std::shared_ptr<Factory> create(const std::string &name)
    {
        if (name == "水果")
        {
            return std::make_shared<FruitFactory>();
        }
        else
        {
            return std::make_shared<AnimalFactory>();
        }
    }
};

int main()
{
    // std::shared_ptr<Fruit> fruit = FruitFactory::create("苹果");
    // fruit->name();

    // std::shared_ptr<FruitFactory> ff(new AppleFacctory());
    // std::shared_ptr<Fruit> fruit = ff->create();
    // fruit->name();
    std::shared_ptr<Factory> ff = FactoryProducer::create("水果");
    std::shared_ptr<Fruit> fruit = ff->getFruit("苹果");
    fruit->name();
    return 0;
}