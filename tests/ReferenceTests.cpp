#include <catch2/catch_all.hpp>

namespace
{

class A
{
private:
    int a = 99;  // NOLINT

public:
    A() = default;
    ~A() = default;
    int geta() const
    {
        return a;
    }
    void seta(int a)
    {
        this->a = a;
    }
};

class B
{
private:
    A& a;

public:
    B() = delete;
    explicit B(A& parent) : a(parent){};
    ~B() = default;
};

}  // Anonymous namespace

TEST_CASE("Constructor tests", "[asde]")
{

    A a{};
    B b(a);
}

TEST_CASE("A reference cannot be reassigned")
{
    A a{};      // a = 99
    A& ref{a};  // a = 99
    CHECK(ref.geta() == a.geta());

    CHECK(&ref == &a);

    A b{};       // a = 99
    b.seta(11);  // a = 11

    ref = b; // sets ref.a = a.a = b.a = 11

    b.seta(22);  // a = 22

    CHECK(&ref != &b);

    CHECK(ref.geta() == 11);
    CHECK(a.geta() == 11);
    CHECK(b.geta() == 22);
}