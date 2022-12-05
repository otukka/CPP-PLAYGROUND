#ifndef __CONSTRUCTORCLASS_H__
#define __CONSTRUCTORCLASS_H__

#include <iostream>
#include <iomanip> /* std::setw*/
#include <cassert> /* std::setw */
#include <vector>
#include <string>
#include <array>
#include <algorithm>

namespace Constructors
{

#define FORGOTTEN_NUMBER 255

#ifndef PRINT_CONSTRUCTORS_WHEN_USED
#define PRINT_CONSTRUCTORS_WHEN_USED 0
#endif

void printHelper(std::string msg)
{
#if (PRINT_CONSTRUCTORS_WHEN_USED == 1)
    std::cout << msg;
#endif
}

enum Constructor
{
    Default = 0,
    rvalueParam,
    lvalueParam,
    Copy,
    CopyAssign,
    Move,
    MoveAssign,
    Swap,
    Forgotten,
    SIZE

};

static std::array<std::string, Constructor::SIZE + 1> ConstructorNames {
    "Default", "rvalueParam", "lvalueParam", "Copy", "CopyAssign", "Move", "MoveAssign", "Swap", "Forgotten", "ILLEGAL"
};

struct State
{
    uint8_t m_order;
    uint8_t m_data;
    uint8_t value;
    Constructor m_constructor;
};

class ConstructorClass
{
private:
    uint8_t m_order;
    uint8_t m_data;
    uint8_t* m_ptr;
    Constructor m_constructor;

public:
    // // Default constructor
    ConstructorClass() : m_order(0), m_data(0), m_constructor(Constructor::Default)
    {
        printHelper("Default constructor\n");

        m_ptr = new uint8_t();
        *m_ptr = 0;
    }

    // Default destructor
    ~ConstructorClass()
    {

        printHelper("Destructor\n");

        delete m_ptr;
    }

    // lvalue constructor
    ConstructorClass(const int& order, const int& data) :
        m_order(order), m_data(data), m_constructor(Constructor::lvalueParam)
    {

        printHelper("lvalue constructor\n");

        m_ptr = new uint8_t();
        *m_ptr = 0;
    }

    // rvalue constructor
    ConstructorClass(const int&& order, const int&& data) :
        m_order(order), m_data(data), m_constructor(Constructor::rvalueParam)
    {

        printHelper("rvalue constructor\n");

        m_ptr = new uint8_t();
        *m_ptr = 0;
    }

    // Copy constructor (lvalue)
    ConstructorClass(const ConstructorClass& other) :
        m_order(other.m_order), m_data(other.m_data), m_constructor(Constructor::Copy)
    {

        printHelper("Copy constructor\n");

        m_ptr = new uint8_t();
        *m_ptr = *other.m_ptr;
    }

    // Copy operator (lvalue)
    ConstructorClass& operator=(const ConstructorClass& other)
    {

        printHelper("Copy operator\n");
        ConstructorClass tmp(other);
        swap(*this, tmp);
        this->m_constructor = Constructor::CopyAssign;
        return *this;
    }

    // Move constructor (rvalue)
    ConstructorClass(ConstructorClass&& other) noexcept :
        m_order(std::move(other.m_order)), m_data(std::move(other.m_data)), m_ptr(std::move(other.m_ptr)),
        m_constructor(Constructor::Move)
    {

        printHelper("Move constructor\n");

        other.m_data = FORGOTTEN_NUMBER;
        other.m_order = FORGOTTEN_NUMBER;
        other.m_ptr = nullptr;
        other.m_constructor = Constructor::Forgotten;
    }

    // Move operator (lvalue)
    ConstructorClass& operator=(ConstructorClass&& other)
    {

        printHelper("Move operator\n");

        if (this != &other)
        {

            // Move data
            *m_ptr = std::move(*other.m_ptr);
            m_order = std::move(other.m_order);
            m_data = std::move(other.m_data);
            m_constructor = Constructor::MoveAssign;

            // Cleanup
            other.m_data = FORGOTTEN_NUMBER;
            other.m_order = FORGOTTEN_NUMBER;
            delete other.m_ptr;
            other.m_ptr = nullptr;
            other.m_constructor = Constructor::Forgotten;
        }
        return *this;
    }

    friend void swap(ConstructorClass& a, ConstructorClass& b)
    {

        printHelper("Swap\n");

        std::swap(a.m_data, b.m_data);
        std::swap(a.m_order, b.m_order);
        std::swap(a.m_ptr, b.m_ptr);
        a.m_constructor = Constructor::Swap;
        b.m_constructor = Constructor::Swap;
    }

    ConstructorClass operator+(const int& data)
    {
        return ConstructorClass(this->m_order, this->m_data + data);
    }

    ConstructorClass operator-(const int& data)
    {
        return ConstructorClass(this->m_order, this->m_data - data);
    }

    ConstructorClass operator+(const int&& data)
    {
        return ConstructorClass(this->m_order, this->m_data + data);
    }

    ConstructorClass operator-(const int&& data)
    {
        return ConstructorClass(this->m_order, this->m_data - data);
    }

    bool checkState(State state)
    {

        return (this->m_data == state.m_data && this->m_order == state.m_order && *this->m_ptr == state.value
                && this->m_constructor == state.m_constructor);
    }

    bool isFrogotten()
    {
        return m_constructor == Constructor::Forgotten;
    }

    bool _checkState(State state)
    {
        std::cout << *this << std::endl;
        return checkState(state);
    }

    int getOrder()
    {
        return this->m_order;
    }
    int getData()
    {
        return this->m_data;
    }
    void setOrder(const int& order)
    {
        m_order = order;
    }
    void setData(const int& data)
    {
        m_data = data;
    }
    void setOrder(const int&& order)
    {
        m_order = order;
    }
    void setData(const int&& data)
    {
        m_data = data;
    }
    friend auto operator<<(std::ostream& os, ConstructorClass const& m) -> std::ostream&
    {
        if (m.m_ptr == nullptr)
        {
            return os << std::left << std::setw(15) << "Order: " << std::setw(3) << static_cast<int>(m.m_order)
                      << std::left << std::setw(15) << "Data: " << std::left << std::setw(3)
                      << static_cast<int>(m.m_data) << std::left << std::setw(15) << "ptr == nullptr" << std::left
                      << std::setw(15) << "Constructor: " << ConstructorNames[m.m_constructor];
        }
        return os << std::left << std::setw(15) << "Order: " << std::setw(3) << static_cast<int>(m.m_order) << std::left
                  << std::setw(15) << "Data: " << std::left << std::setw(3) << static_cast<int>(m.m_data) << std::left
                  << std::setw(15) << "ptr address : " << std::setw(20) << m.m_ptr << std::left << std::setw(15)
                  << "ptr value : " << std::setw(3) << *m.m_ptr << std::left << std::setw(15)
                  << "Constructor: " << ConstructorNames[m.m_constructor];
    }
};

class DerivedConstructorClass : public ConstructorClass
{

public:
    DerivedConstructorClass() = delete;
    ~DerivedConstructorClass();
};

std::string value(ConstructorClass& mc)
{
    return "lvalue";
}

std::string value(ConstructorClass&& mc)
{
    return "rvalue";
}
std::string value(const ConstructorClass& mc)
{
    return "lvalue";
}

std::string value(const ConstructorClass&& mc)
{
    return "rvalue";
}
}

#endif // __CONSTRUCTORCLASS_H__