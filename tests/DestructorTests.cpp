
#include <catch2/catch_all.hpp>
#include <memory>
#include <iostream>  // cout

class C
{
private:
    int running;

public:
    C() : running{1}
    {
        std::cout << "Constructor C" << std::endl;
    };
    ~C()
    {
        std::cout << "Destructor C " << running << std::endl;
        running = 0;
    };
    int getRunning()
    {
        return running;
    }
};
class B
{
private:
    int running;

public:
    B() : running{1}
    {
        std::cout << "Constructor B" << std::endl;
    };
    ~B()
    {
        std::cout << "Destructor B " << running << std::endl;
        running = 0;
    };
    int getRunning()
    {
        return running;
    }
};
class A
{
private:
    B b;
    C c;

public:
    A() : b{}, c{}
    {
        std::cout << "Constructor A" << std::endl;
    };
    ~A()
    {
        std::cout << "Destructor A" << std::endl;
        // b.~B(); // obsolete
        // c.~C(); // obsolete
    };
};

class A_shared
{
private:
    std::shared_ptr<B> b;
    std::shared_ptr<C> c;

public:
    A_shared()
    {
        b = std::make_shared<B>();
        c = std::make_shared<C>();
        std::cout << "Constructor A_shared" << std::endl;
    };
    ~A_shared()
    {
        std::cout << "Destructor A_shared" << std::endl;
    };
    std::shared_ptr<C> getC()
    {
        return c;
    }
};

class A_unique
{
private:
    std::unique_ptr<B> b;
    std::unique_ptr<C> c;

public:
    A_unique()
    {
        b = std::make_unique<B>();
        c = std::make_unique<C>();
        std::cout << "Constructor A_unique" << std::endl;
    };
    ~A_unique()
    {
        std::cout << "Destructor A_unique" << std::endl;
        b.reset(nullptr);
        c.reset(nullptr);
    };
};

class A_mixed
{
private:
    std::unique_ptr<B> b;
    std::shared_ptr<C> c;

public:
    A_mixed()
    {
        b = std::make_unique<B>();
        c = std::make_shared<C>();
        std::cout << "Constructor A_mixed" << std::endl;
    };
    ~A_mixed()
    {
        std::cout << "Destructor A_mixed" << std::endl;
        // b.reset(nullptr); // Would destroy b always first
    };
    std::shared_ptr<C> getC() const
    {
        return c;
    }
};

TEST_CASE("Case 1")
{
    A a{};
}
TEST_CASE("Case 2")
{
    A_shared a{};
}
TEST_CASE("Case 2.5")
{
    std::shared_ptr<C> tmp;
    {
        A_shared a{};
        tmp = a.getC();  // A_shared::c is co-owned by this so this will delay A_shared::c destruction
    }
}
TEST_CASE("Case 3")
{
    A_unique a{};
}
TEST_CASE("Case 4")
{
    A_mixed a{};
}
TEST_CASE("Case 4.5")
{
    std::shared_ptr<C> tmp;
    {
        A_mixed a{};
        tmp = a.getC();  // A_mixed::c is co-owned by this so this will delay A_mixed::c destruction
    }
}