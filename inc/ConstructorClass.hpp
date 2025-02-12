#ifndef CONSTRUCTORCLASS_H
#define CONSTRUCTORCLASS_H

#include <iostream>
#include <iomanip> /* std::setw*/
#include <cassert> /* std::setw */
#include <vector>
#include <string>
#include <array>
#include <algorithm>

namespace Constructors
{

#define FORGOTTEN_NUMBER (int16_t)255

#ifndef PRINT_CONSTRUCTORS_WHEN_USED
#define PRINT_CONSTRUCTORS_WHEN_USED 0
#endif

void printHelper(std::string msg)
{
#if (PRINT_CONSTRUCTORS_WHEN_USED == 1)
    std::cout << msg;
#else
    (void)msg;
#endif
}

enum Constructor : int16_t
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

static std::array<std::string, Constructor::SIZE + 1> ConstructorNames{
    "Default", "rvalueParam", "lvalueParam", "Copy",      "CopyAssign",
    "Move",    "MoveAssign",  "Swap",        "Forgotten", "ILLEGAL"};

// Overloading ostream operator for printing Constructor enum
std::ostream& operator<<(std::ostream& os, const Constructor& constructor)
{
    if (static_cast<size_t>(constructor) < ConstructorNames.size())
    {
        return os << ConstructorNames[static_cast<size_t>(constructor)];
    }
    else
    {
        return os << "ILLEGAL";
    }
}

struct State
{
    int16_t m_order;
    int16_t m_data;
    int16_t m_value;
    Constructor m_constructor;
};

class ConstructorClass
{
private:
    int16_t m_order;
    int16_t m_data;
    int16_t* m_value;
    Constructor m_constructor;

public:
    // Default constructor
    ConstructorClass() : m_order(), m_data(), m_value(new int16_t(0)), m_constructor(Constructor::Default)
    {
        printHelper("Default constructor\n");
    }

    // Default destructor
    ~ConstructorClass()
    {
        printHelper("Destructor\n");
        delete m_value;
    }

    // lvalue constructor
    ConstructorClass(const int16_t& order, const int16_t& data) :
        m_order(order), m_data(data), m_value(new int16_t(0)), m_constructor(Constructor::lvalueParam)
    {
        printHelper("lvalue constructor\n");
    }

    // rvalue constructor
    ConstructorClass(const int16_t&& order, const int16_t&& data) :
        m_order(order), m_data(data), m_constructor(Constructor::rvalueParam)
    {

        printHelper("rvalue constructor\n");

        m_value = new int16_t();
        *m_value = 0;
    }

    // Copy constructor (lvalue)
    ConstructorClass(const ConstructorClass& other) :
        m_order(other.m_order), m_data(other.m_data), m_value(new int16_t(*other.m_value)),
        m_constructor(Constructor::Copy)
    {
        printHelper("Copy constructor\n");
    }

    // Copy assignment operator (lvalue)
    ConstructorClass& operator=(const ConstructorClass& other)
    {

        printHelper("Copy assignment operator\n");
        ConstructorClass tmp(other);
        swap(*this, tmp);
        this->m_constructor = Constructor::CopyAssign;
        return *this;
    }

    // Move constructor (rvalue)
    ConstructorClass(ConstructorClass&& other) noexcept :
        m_order(std::move(other.m_order)), m_data(std::move(other.m_data)), m_value(std::move(other.m_value)),
        m_constructor(Constructor::Move)
    {

        printHelper("Move constructor\n");

        other.m_data = FORGOTTEN_NUMBER;
        other.m_order = FORGOTTEN_NUMBER;
        other.m_value = nullptr;
        other.m_constructor = Constructor::Forgotten;
    }

    // Move assignment operator (lvalue)
    ConstructorClass& operator=(ConstructorClass&& other)
    {

        printHelper("Move assignment operator\n");

        if (this != &other)
        {

            // Move data
            *m_value = std::move(*other.m_value);
            m_order = std::move(other.m_order);
            m_data = std::move(other.m_data);
            m_constructor = Constructor::MoveAssign;

            // Cleanup
            other.m_data = FORGOTTEN_NUMBER;
            other.m_order = FORGOTTEN_NUMBER;
            delete other.m_value;
            other.m_value = nullptr;
            other.m_constructor = Constructor::Forgotten;
        }
        return *this;
    }

    friend void swap(ConstructorClass& a, ConstructorClass& b)
    {

        printHelper("Swap\n");

        std::swap(a.m_data, b.m_data);
        std::swap(a.m_order, b.m_order);
        std::swap(a.m_value, b.m_value);
        a.m_constructor = Constructor::Swap;
        b.m_constructor = Constructor::Swap;
    }

    ConstructorClass operator+(const int16_t& data)
    {
        return ConstructorClass(this->m_order, this->m_data + data);
    }

    ConstructorClass operator-(const int16_t& data)
    {
        return ConstructorClass(this->m_order, this->m_data - data);
    }

    ConstructorClass operator+(const int16_t&& data)
    {
        return ConstructorClass(this->m_order, this->m_data + data);
    }

    ConstructorClass operator-(const int16_t&& data)
    {
        return ConstructorClass(this->m_order, this->m_data - data);
    }

    bool isFrogotten()
    {
        return m_constructor == Constructor::Forgotten;
    }

    Constructor getConstructor() const
    {
        return this->m_constructor;
    }
    int16_t getOrder() const
    {
        return this->m_order;
    }
    int16_t getData() const
    {
        return this->m_data;
    }
    int16_t getValue() const
    {
        return *this->m_value;
    }

    void setConstructor(const Constructor& constructor)
    {
        m_constructor = constructor;
    }
    void setOrder(const int16_t& order)
    {
        m_order = order;
    }
    void setData(const int16_t& data)
    {
        m_data = data;
    }
    void setValue(const int16_t& value)
    {
        *m_value = value;
    }
    void setValue(int16_t* value)
    {
        delete m_value;
        m_value = value;
    }
};

class DerivedConstructorClass : public ConstructorClass
{

public:
    DerivedConstructorClass() = delete;
    ~DerivedConstructorClass();
};

std::string lvalueORrvalue(ConstructorClass& mc)
{
    (void)mc;
    return "lvalue";
}

std::string lvalueORrvalue(ConstructorClass&& mc)
{
    (void)mc;
    return "rvalue";
}
std::string lvalueORrvalue(const ConstructorClass& mc)
{
    (void)mc;
    return "lvalue";
}

std::string lvalueORrvalue(const ConstructorClass&& mc)
{
    (void)mc;
    return "rvalue";
}
}

#endif  // CONSTRUCTORCLASS_H