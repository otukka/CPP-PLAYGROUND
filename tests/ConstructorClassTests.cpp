#include <catch2/catch_all.hpp>

#include "ConstructorClass.hpp"

namespace
{
using namespace Constructors;

class ConstructorClassWrapper
{
public:
    ConstructorClassWrapper() : sub1{}
    {
        sub2 = ConstructorClass();
    }

    ~ConstructorClassWrapper() = default;

    ConstructorClass sub1;
    ConstructorClass sub2;
};

}

#define CHECK_STATE(obj, order, data, value, constructor)                                                              \
    CHECK(obj.getOrder() == order);                                                                                    \
    CHECK(obj.getData() == data);                                                                                      \
    CHECK(obj.getValue() == value);                                                                                    \
    CHECK(obj.getConstructor() == constructor);

#define CHECK_PTR_STATE(obj, order, data, value, constructor)                                                          \
    CHECK(obj->getOrder() == order);                                                                                   \
    CHECK(obj->getData() == data);                                                                                     \
    CHECK(obj->getValue() == value);                                                                                   \
    CHECK(obj->getConstructor() == constructor);

/* Tests */
TEST_CASE("lvalue or rvalue function")
{
    ConstructorClass tmp1 = ConstructorClass();
    CHECK(lvalueORrvalue(tmp1) == "lvalue");

    CHECK(lvalueORrvalue((ConstructorClass&&)tmp1) == "rvalue");

    CHECK(lvalueORrvalue(std::move(tmp1)) == "rvalue");

    CHECK(lvalueORrvalue(tmp1 + 10) == "rvalue");

    CHECK(lvalueORrvalue(tmp1 - 10) == "rvalue");

    int16_t tmp_value = 10U;
    CHECK(lvalueORrvalue(tmp1 + tmp_value) == "rvalue");
    CHECK(lvalueORrvalue(tmp1 - tmp_value) == "rvalue");

    ConstructorClass tmp2 = ConstructorClass();
    ConstructorClass tmp3 = tmp2;
    CHECK(lvalueORrvalue(tmp3) == "lvalue");
    CHECK(lvalueORrvalue((ConstructorClass&)tmp3) == "lvalue");

    ConstructorClass tmp4 = ConstructorClass();
    CHECK(lvalueORrvalue(tmp4) == "lvalue");

    ConstructorClass tmp5 = tmp4 + 10;
    CHECK(lvalueORrvalue(tmp5) == "lvalue");

    CHECK(lvalueORrvalue((ConstructorClass&)tmp4) == "lvalue");

    const ConstructorClass tmp6 = ConstructorClass();
    CHECK(lvalueORrvalue(tmp4) == "lvalue");
}

TEST_CASE("parenthesis constructors")
{
    ConstructorClass tmp1 = ConstructorClass();
    CHECK_STATE(tmp1, 0, 0, 0, Constructor::Default);

    // ConstructorClass tmp(); // Invalid

    ConstructorClass tmp2(tmp1);
    CHECK_STATE(tmp2, 0, 0, 0, Constructor::Copy);

    ConstructorClass tmp3 = ConstructorClass(tmp1);
    CHECK_STATE(tmp3, 0, 0, 0, Constructor::Copy);

    ConstructorClass tmp4(10, 10);
    CHECK_STATE(tmp4, 10, 10, 0, Constructor::rvalueParam);

    ConstructorClass tmp5 = ConstructorClass(1, 1);
    CHECK_STATE(tmp5, 1, 1, 0, Constructor::rvalueParam);

    int16_t order = 1;
    int16_t data = 1;

    ConstructorClass tmp6 = ConstructorClass(order, data);
    CHECK_STATE(tmp6, order, data, 0, Constructor::lvalueParam);

    ConstructorClass tmp7(order, data);
    CHECK_STATE(tmp7, order, data, 0, Constructor::lvalueParam);
}

TEST_CASE("braced constructor")
{
    ConstructorClass tmp1 = ConstructorClass{};
    CHECK_STATE(tmp1, 0, 0, 0, Constructor::Default);

    ConstructorClass tmp2{};
    CHECK_STATE(tmp2, 0, 0, 0, Constructor::Default);

    ConstructorClass tmp3 = {};
    CHECK_STATE(tmp3, 0, 0, 0, Constructor::Default);

    ConstructorClass tmp4 = ConstructorClass{tmp3};
    CHECK_STATE(tmp4, 0, 0, 0, Constructor::Copy);
}

// Move tests
TEST_CASE("move constructor")
{
    {
        ConstructorClass tmp1 = ConstructorClass(1, 10);
        ConstructorClass tmp2 = std::move(tmp1);
        CHECK_STATE(tmp2, 1, 10, 0, Constructor::Move);
        REQUIRE(tmp1.isFrogotten());
    }
    {
        ConstructorClass tmp1 = ConstructorClass();
        ConstructorClass tmp2 = (ConstructorClass&&)tmp1;
        CHECK_STATE(tmp2, 0, 0, 0, Constructor::Move);
        REQUIRE(tmp1.isFrogotten());
    }
    {
        ConstructorClass tmp1 = ConstructorClass();
        ConstructorClass tmp2(std::move(tmp1));
        CHECK_STATE(tmp2, 0, 0, 0, Constructor::Move);
        REQUIRE(tmp1.isFrogotten());
    }
    {
        ConstructorClass tmp1 = ConstructorClass();
        CHECK_STATE(tmp1, 0, 0, 0, Constructor::Default);
        ConstructorClass tmp2 = std::move(tmp1);
        CHECK_STATE(tmp2, 0, 0, 0, Constructor::Move);
        REQUIRE(!tmp1.isFrogotten());  // rvalue move i.e. not forgotten
    }
}

TEST_CASE("move assign constructor")
{
    ConstructorClass tmp0 = ConstructorClass();
    ConstructorClass tmp1 = ConstructorClass();
    tmp0 = std::move(tmp1);
    CHECK_STATE(tmp0, 0, 0, 0, Constructor::MoveAssign);
    REQUIRE(tmp1.isFrogotten());
}

// Copy tests
TEST_CASE("copy constructor")
{
    {
        ConstructorClass tmp1 = ConstructorClass();
        ConstructorClass tmp2 = tmp1;
        CHECK_STATE(tmp1, 0, 0, 0, Constructor::Default);
        CHECK_STATE(tmp2, 0, 0, 0, Constructor::Copy);
    }
    {
        const ConstructorClass tmp1 = ConstructorClass();
        ConstructorClass tmp2 = tmp1;
        CHECK_STATE(tmp1, 0, 0, 0, Constructor::Default);
        CHECK_STATE(tmp2, 0, 0, 0, Constructor::Copy);
    }
    {
        ConstructorClass tmp1 = ConstructorClass();
        ConstructorClass tmp2 = ConstructorClass(tmp1);
        CHECK_STATE(tmp1, 0, 0, 0, Constructor::Default);
        CHECK_STATE(tmp2, 0, 0, 0, Constructor::Copy);
    }
    {
        const ConstructorClass tmp1 = ConstructorClass();
        ConstructorClass tmp2 = ConstructorClass(tmp1);
        CHECK_STATE(tmp1, 0, 0, 0, Constructor::Default);
        CHECK_STATE(tmp2, 0, 0, 0, Constructor::Copy);
    }
}

TEST_CASE("copy assign constructor")
{
    ConstructorClass tmp0 = ConstructorClass();
    ConstructorClass tmp1 = ConstructorClass();
    tmp0 = tmp1;
    CHECK_STATE(tmp0, 0, 0, 0, Constructor::CopyAssign);
}

TEST_CASE("getters and setters")
{
    ConstructorClass tmp = ConstructorClass();

    tmp.setData(12);
    CHECK(tmp.getData() == 12);

    tmp.setOrder(105);
    CHECK(tmp.getOrder() == 105);

    tmp.setConstructor(Constructor::Forgotten);
    CHECK(tmp.getConstructor() == Constructor::Forgotten);

    tmp.setValue(42);
    CHECK(tmp.getValue() == 42);

    int16_t* ptr = new int16_t(89);
    tmp.setValue(ptr);
    CHECK(tmp.getValue() == 89);
}

TEST_CASE("Operators")
{
    ConstructorClass tmp1 = ConstructorClass();
    tmp1 = tmp1 + 10;
    CHECK_STATE(tmp1, 0, 10, 0, Constructor::MoveAssign);

    ConstructorClass tmp2 = ConstructorClass();
    tmp2 = tmp1 - 1;
    CHECK_STATE(tmp2, 0, 9, 0, Constructor::MoveAssign);

    ConstructorClass tmp3 = ConstructorClass();
    int16_t plus = 10;
    tmp3 = tmp3 + plus;
    CHECK_STATE(tmp3, 0, plus, 0, Constructor::MoveAssign);
}

TEST_CASE("WithVector")
{
    ConstructorClass tmp1 = ConstructorClass();

    std::vector<ConstructorClass> v;
    v.reserve(10);
    v.push_back(tmp1);
    v.push_back(std::move(tmp1));

    CHECK_STATE(v.at(0), 0, 0, 0, Constructor::Copy);
    CHECK_STATE(v.at(1), 0, 0, 0, Constructor::Move);
}

TEST_CASE("wrapped class")
{
    ConstructorClassWrapper tmp{};
    CHECK_STATE(tmp.sub1, 0, 0, 0, Constructor::Default);
    CHECK_STATE(tmp.sub2, 0, 0, 0, Constructor::MoveAssign);
}

TEST_CASE("using")
{
    using CC = ConstructorClass;
    ConstructorClass tmp = CC();
    CHECK_STATE(tmp, 0, 0, 0, Constructor::Default);
}

TEST_CASE("new")
{
    {
        auto tmp = new ConstructorClass();
        CHECK_PTR_STATE(tmp, 0, 0, 0, Constructor::Default);
    }
    {
        auto tmp = *new ConstructorClass();
        CHECK_STATE(tmp, 0, 0, 0, Constructor::Copy);
    }
}

TEST_CASE("shared ptr")
{
    auto tmp = std::make_shared<ConstructorClass>();

    CHECK_PTR_STATE(tmp.get(), 0, 0, 0, Constructor::Default);
}
