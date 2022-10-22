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

static std::array<std::string, Constructor::SIZE + 1> ConstructorNames{
"Default",
"rvalueParam",
"lvalueParam",
"Copy",
"CopyAssign",
"Move",
"MoveAssign",
"Swap",
"Forgotten",
"ILLEGAL"
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
    uint8_t *m_ptr;
    Constructor m_constructor;

public:

    // // Default constructor
    ConstructorClass() : m_order(0), m_data(0),
                m_constructor(Constructor::Default)
    {
        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "Default constructor\n";
        #endif

        m_ptr = new uint8_t();
        *m_ptr = 0;
    }


    // Default destructor
    ~ConstructorClass()
    {
        
        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "Destructor\n";
        #endif

        delete m_ptr;
    }
    

    // lvalue constructor
    ConstructorClass(const int &a_order, const int &a_data) : m_order(a_order), m_data(a_data), m_constructor(Constructor::lvalueParam)
    {
        
        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "lvalue constructor\n";
        #endif

        m_ptr = new uint8_t();
        *m_ptr = 0;
    }

    // rvalue constructor
    ConstructorClass(const int &&a_order, const int &&a_data) : m_order(a_order), m_data(a_data), m_constructor(Constructor::rvalueParam)
    {
        
        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "rvalue constructor\n";
        #endif

        m_ptr = new uint8_t();
        *m_ptr = 0;
    }

    // Copy constructor (lvalue)
    ConstructorClass(const ConstructorClass &other) : m_order(other.m_order), m_data(other.m_data), m_constructor(Constructor::Copy)
    {
        
        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "Copy constructor\n";
        #endif

        m_ptr = new uint8_t();
        *m_ptr = *other.m_ptr;
    }

    // Copy operator (lvalue)
    ConstructorClass &operator=(ConstructorClass &other)
    {
        

        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "Copy operator\n";
        #endif

        swap(*this, other);
        this->m_constructor = Constructor::CopyAssign;
        return *this;
    }

    // Move constructor (rvalue)
    ConstructorClass(ConstructorClass &&other) noexcept : m_order(std::move(other.m_order)), m_data(std::move(other.m_data)), m_ptr(std::move(other.m_ptr)), m_constructor(Constructor::Move)
    {

        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "Move constructor\n";
        #endif

        other.m_data = FORGOTTEN_NUMBER;
        other.m_order = FORGOTTEN_NUMBER;
        other.m_ptr = nullptr;
        other.m_constructor = Constructor::Forgotten;
    }



    // Move operator (lvalue)
    ConstructorClass &operator=(ConstructorClass &&other)
    {


        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "Move operator\n";
        #endif

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

        #if(PRINT_CONSTRUCTORS_WHEN_USED == 1)
        std::cout << "Swap\n";
        #endif

        std::swap(a.m_data, b.m_data);
        std::swap(a.m_order, b.m_order);
        std::swap(a.m_ptr, b.m_ptr);
        a.m_constructor = Constructor::Swap;
        b.m_constructor = Constructor::Swap;
    }

    ConstructorClass operator+(const int &data)
    {
        return ConstructorClass(this->m_order, this->m_data + data);
    }

    ConstructorClass operator-(const int &data)
    {
        return ConstructorClass(this->m_order, this->m_data - data);
    }

    ConstructorClass operator+(const int &&data)
    {
        return ConstructorClass(this->m_order, this->m_data + data);
    }

    ConstructorClass operator-(const int &&data)
    {
        return ConstructorClass(this->m_order, this->m_data - data);
    }

    bool checkState(State a_state)
    {

        if (this->m_data == a_state.m_data && this->m_order == a_state.m_order && *this->m_ptr == a_state.value && this->m_constructor == a_state.m_constructor)
        {
            return true;
        }
        return false;
    }

    bool isFrogotten()
    {
        return m_constructor == Constructor::Forgotten;
    }

    bool _checkState(State a_state)
    {
        std::cout << *this << std::endl;
        return checkState(a_state);
    }

    int getOrder()
    {
        return this->m_order;
    }
    int getData()
    {
        return this->m_data;
    }
    void setOrder(const int &a_order)
    {
        m_order = a_order;
    }
    void setData(const int &a_data)
    {
        m_data = a_data;
    }
    void setOrder(const int &&a_order)
    {
        m_order = a_order;
    }
    void setData(const int &&a_data)
    {
        m_data = a_data;
    }
    friend auto operator<<(std::ostream &os, ConstructorClass const &m) -> std::ostream &
    {
        if (m.m_ptr == nullptr)
        {
            return os << std::left << std::setw(15) << "Order: " << std::setw(3) << static_cast<int>(m.m_order) << std::left << std::setw(15) << "Data: " << std::left << std::setw(3) << static_cast<int>(m.m_data) << std::left << std::setw(15) << "ptr == nullptr" << std::left << std::setw(15) << "Constructor: " << ConstructorNames[m.m_constructor];
        }
        return os << std::left << std::setw(15) << "Order: " << std::setw(3) << static_cast<int>(m.m_order) << std::left << std::setw(15) << "Data: " << std::left << std::setw(3) << static_cast<int>(m.m_data) << std::left << std::setw(15) << "ptr address : " << std::setw(20) << m.m_ptr << std::left << std::setw(15) << "ptr value : " << std::setw(3) << *m.m_ptr << std::left << std::setw(15) << "Constructor: " << ConstructorNames[m.m_constructor];
    }
};


class DerivedConstructorClass : public ConstructorClass
{

public:
    DerivedConstructorClass() = delete;
    ~DerivedConstructorClass();
};



std::string value(ConstructorClass &mc)
{
    return "lvalue";
}

std::string value(ConstructorClass &&mc)
{
    return "rvalue";
}
std::string value(const ConstructorClass &mc)
{
    return "lvalue";
}

std::string value(const ConstructorClass &&mc)
{
    return "rvalue";
}


}

#endif // __CONSTRUCTORCLASS_H__