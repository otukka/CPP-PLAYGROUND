#include <catch2/catch_all.hpp>
#include <memory>
#include <iostream>

namespace
{
class A;

class B
{
private:
    std::shared_ptr<A> a;

public:
    B() = default;
    ~B() = default;

    // B(const B& other) : a{nullptr}
    // {
    //     // std::cout << "B: Copy constructor" << std::endl;
    // }
    // B(B&& other) noexcept : a{nullptr}
    // {
    //     // std::cout << "B: Move constructor" << std::endl;
    // }

    // B& operator=(const B& other)
    // {
    //     // std::cout << "B: Copy operator" << std::endl;
    //     B copy{other};
    //     swap(*this, copy);
    //     return *this;
    // }

    B& operator=(B&& other) noexcept
    {
        // std::cout << "B: Move operator" << std::endl;
        this->a = std::move(other.a);
        return *this;
    }

    friend void swap(B& b1, B& b2)
    {
        using std::swap;
        swap(b1.a, b2.a);
    }

    std::shared_ptr<A> getA()
    {
        return a;
    }
    void createA(A* a)
    {
        this->a = std::shared_ptr<A>{a};
    }
    void createA(std::shared_ptr<A> a)
    {
        this->a = a;
    }
    // std::shared_ptr<B> createB()
    // {
    //     return std::make_shared<B>();
    // }
    void setA(std::shared_ptr<A> a)
    {
        this->a = a;
    }
    static const char* msg()
    {
        return "B";
    }
};

class A
{
private:
    std::shared_ptr<B> b;

public:
    A() = default;
    ~A() = default;

    A(const A& other) : b(other.b)
    {
        // std::cout << "A: Copy constructor" << std::endl;
    }
    A(A&& other) noexcept : b{std::move(other.b)}
    {
        // std::cout << "A: Move constructor" << std::endl;
    }

    A& operator=(const A& other)
    {
        // std::cout << "A: Copy operator" << std::endl;
        A copy{other};
        swap(*this, copy);
        return *this;
    }

    A& operator=(A&& other) noexcept
    {
        // std::cout << "A: Move operator" << std::endl;
        this->b = std::move(other.b);
        return *this;
    }

    friend void swap(A& a1, A& a2)
    {
        // std::cout << "A: Swap operator" << std::endl;
        using std::swap;
        swap(a1.b, a2.b);
    }

    std::shared_ptr<B> getB()
    {
        return b;
    }
    void createB(B* b)
    {
        this->b = std::shared_ptr<B>{b};
    }
    void createB(std::shared_ptr<B> b)
    {
        this->b = b;
    }
    std::shared_ptr<A> createA()
    {
        return std::make_shared<A>(*this);
    }
    void setB(std::shared_ptr<B> b)
    {
        this->b = b;
    }

    static const char* msg()
    {
        return "A";
    }
};

// void func(std::unique_ptr<std::string>&& s)
// {
//     // std::cout << "qweerty " << s.get() << std::endl;
// }

}  // Anonymous namespace

// TEST_CASE("Smart Pointer reference tests")
// {
//     auto s = std::make_unique<std::string>("asdf");
//     func(std::move(s));
// }

TEST_CASE("Smart Pointer constructor tests")
{

    {
        A a{};
        auto ptr = std::make_shared<A>(a);
        CHECK(ptr.get() != nullptr);
        CHECK(ptr->msg() == std::string("A"));
    }
    {
        B b{};
        // auto ptr = std::make_shared<B>(b);
        // CHECK(ptr.get() != nullptr);
        // CHECK(ptr->msg() == std::string("B"));
    }
    {
        std::shared_ptr<A> ptr{new A};
        CHECK(ptr.get() != nullptr);
        CHECK(ptr->msg() == std::string("A"));
    }
    {
        std::shared_ptr<B> ptr{new B};
        CHECK(ptr.get() != nullptr);
        CHECK(ptr->msg() == std::string("B"));
    }
}

TEST_CASE("Smart Pointer A copy constructor")
{
    A a{};
    std::shared_ptr<B> b{new B};
    CHECK(b.use_count() == 1);
    a.setB(b);
    CHECK(b.use_count() == 2);
    A Acopy{a};
    CHECK(Acopy.getB().get() == b.get());
    CHECK(b.use_count() == 3);
}

TEST_CASE("Smart Pointer A move constructor")
{
    A a{};
    std::shared_ptr<B> b{new B};
    CHECK(b.use_count() == 1);
    a.setB(b);
    CHECK(b.use_count() == 2);
    A Amove{std::move(a)};
    CHECK(Amove.getB().get() == b.get());
    CHECK(b.use_count() == 2);
}

TEST_CASE("Smart Pointer A copy assing")
{

    A acopied{};
    std::shared_ptr<B> b{new B};
    CHECK(b.use_count() == 1);
    acopied.setB(b);
    CHECK(b.use_count() == 2);

    A a{};
    a = acopied;
    CHECK(a.getB().get() == b.get());
    CHECK(b.use_count() == 3);
}

TEST_CASE("Smart Pointer A move assing")
{

    A amoved{};
    std::shared_ptr<B> b{new B};
    CHECK(b.use_count() == 1);
    amoved.setB(b);
    CHECK(b.use_count() == 2);

    A a{};
    a = std::move(amoved);
    CHECK(a.getB().get() == b.get());
    CHECK(b.use_count() == 2);
}

TEST_CASE("Smart Pointer createB functions")
{
    {
        A a{};
        B* b{};
        a.createB(b);
        CHECK(a.getB().get() == b);
    }
    {
        A a{};
        B b{};

        std::shared_ptr<A> ptr = a.createA();
        CHECK(ptr.use_count() == 1);
        b.createA(ptr);
        CHECK(b.getA().get() == ptr.get());
        CHECK(ptr.use_count() == 2);
    }
}

TEST_CASE("Smart Pointer createA functions")
{
    {
        A* a{};
        B b{};
        b.createA(a);
        CHECK(b.getA().get() == a);
    }
    {
        A a{};
        B b{};

        // std::shared_ptr<B> ptr = b.createB();
        // CHECK(ptr.use_count() == 1);
        // a.createB(ptr);
        // CHECK(a.getB().get() == ptr.get());
        // CHECK(ptr.use_count() == 2);
    }
}
