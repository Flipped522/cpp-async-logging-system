#include <iostream>
#include <string>

class RentHouse
{
public:
    virtual void rentHouse() = 0;
};

class Landlord : public RentHouse
{
public:
    void rentHouse() override
    {
        std::cout << "将房子租出去" << std::endl;
    }
};

class Intermediary : public RentHouse
{
public:
    void rentHouse() override
    {
        std::cout << "发布招租启示" << std::endl;
        std::cout << "带人看房" << std::endl;
        _landlord.rentHouse();
        std::cout << "负责售后维修" << std::endl;
    }
private:
    Landlord _landlord;
};

int main()
{
    Intermediary intermediary;
    intermediary.rentHouse();

    return 0;
}