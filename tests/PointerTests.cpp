#include <catch2/catch_all.hpp>

namespace
{
class A;

class B
{
private:
    A* a;

public:
    B() = delete;
    explicit B(A* parent) : a(parent){};
    ~B() = default;
    B(const B& other) : a(other.a) {}
    B(const B&& other) noexcept : a(other.a) {}
    B& operator=(const B& other)
    {
        B copy(other.a);
        swap(*this, copy);
        return *this;
    }
    B& operator=(B&& other) noexcept
    {
        this->a = std::move(other.a);
        return *this;
    }

    friend void swap(B& b1, B& b2)
    {
        using std::swap;
        swap(b1.a, b2.a);
    }
};

class A
{
private:
    B b;

public:
    A() : b{this} {};
    ~A() = default;
    A(const A& other) : b(other.b) {}
    A(const A&& other) noexcept : b{std::move(other.b)} {}
    A& operator=(A other)
    {
        swap(*this, other);
        return *this;
    }
    A& operator=(A&& other) noexcept
    {
        this->b = std::move(other.b);
        return *this;
    }

    friend void swap(A& a1, A& a2)
    {
        using std::swap;
        swap(a1.b, a2.b);
    }
};

}  // Anonymous namespace

TEST_CASE("Pointer tests")
{
    A a{};
}