#include <catch2/catch_all.hpp>

#include "ConstructorClass.hpp"


/* Tests */
TEST_CASE("Unit test: testValue")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    REQUIRE(Constructors::value(tmp1) == "lvalue");
    REQUIRE(Constructors::value((Constructors::ConstructorClass &)tmp1) == "lvalue");

    REQUIRE(Constructors::value((Constructors::ConstructorClass &&) tmp1) == "rvalue");
    REQUIRE(Constructors::value(std::move(tmp1)) == "rvalue");
    REQUIRE(Constructors::value(tmp1 + 10) == "rvalue");
    REQUIRE(Constructors::value(tmp1 - 10) == "rvalue");
    int tmp_value = 10;
    REQUIRE(Constructors::value(tmp1 + tmp_value) == "rvalue");
    REQUIRE(Constructors::value(tmp1 - tmp_value) == "rvalue");

    Constructors::ConstructorClass tmp2 = Constructors::ConstructorClass();
    Constructors::ConstructorClass tmp3 = tmp2;
    REQUIRE(Constructors::value(tmp3) == "lvalue");
    REQUIRE(Constructors::value((Constructors::ConstructorClass &)tmp3) == "lvalue");

    Constructors::ConstructorClass tmp4 = Constructors::ConstructorClass();
    Constructors::ConstructorClass tmp5 = tmp4 + 10;
    REQUIRE(Constructors::value(tmp4) == "lvalue");
    REQUIRE(Constructors::value((Constructors::ConstructorClass &)tmp4) == "lvalue");
}

TEST_CASE("Unit test: testDefaultConstructor")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    REQUIRE(tmp1.checkState({0, 0, 0, Constructors::Constructor::Default}));
    REQUIRE(!tmp1.checkState({1, 0, 0, Constructors::Constructor::Default}));
    REQUIRE(!tmp1.checkState({0, 1, 0, Constructors::Constructor::Default}));
    REQUIRE(!tmp1.checkState({0, 0, 1, Constructors::Constructor::Default}));
    REQUIRE(!tmp1.checkState({1, 1, 1, Constructors::Constructor::Default}));

    for (size_t i = Constructors::Constructor::SIZE+1; i < Constructors::Constructor::SIZE; i++)
    {
        /* code */
        REQUIRE(!tmp1.checkState({0, 0, 0, (Constructors::Constructor)i}));
    }
    
}


TEST_CASE("Unit test: testParenthesisConstructor")
{

    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    REQUIRE(tmp1.checkState({0, 0, 0, Constructors::Constructor::Default}));


    Constructors::ConstructorClass tmp2();
    // error: request for member ‘checkState’ in ‘tmp2’, which is of non-class type ‘Constructors::ConstructorClass()
    // REQUIRE(tmp2.checkState({0, 0, 0, Constructors::Constructor::Default}));

    Constructors::ConstructorClass tmp3(tmp1);
    REQUIRE(tmp3.checkState({0, 0, 0, Constructors::Constructor::Copy}));

    auto tmp4 = Constructors::ConstructorClass(tmp1);
    REQUIRE(tmp4.checkState({0, 0, 0, Constructors::Constructor::Copy}));

    Constructors::ConstructorClass tmp5(10, 10);
    REQUIRE(tmp5.checkState({10, 10, 0, Constructors::Constructor::rvalueParam}));


    // error: request for member ‘checkState’ in ‘tmp6’, which is of non-class type ‘MyDerivedClass()’
    // MyDerivedClass tmp6();
    // REQUIRE(tmp6.checkState({0, 0, 0, Constructors::Constructor::Default}));

    Constructors::ConstructorClass tmp7(270, 270);
    REQUIRE(!tmp7.checkState({0, 0, 0, Constructors::Constructor::Default}));

}

TEST_CASE("Unit test: testBraceConstructor")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass{};
    REQUIRE(tmp1.checkState({0, 0, 0, Constructors::Constructor::Default}));

    Constructors::ConstructorClass tmp2{};
    REQUIRE(tmp2.checkState({0, 0, 0, Constructors::Constructor::Default}));

    Constructors::ConstructorClass tmp3 = Constructors::ConstructorClass{};
    REQUIRE(tmp3.checkState({0, 0, 0, Constructors::Constructor::Default}));

    auto tmp4 = Constructors::ConstructorClass{};
    REQUIRE(tmp4.checkState({0, 0, 0, Constructors::Constructor::Default}));

    Constructors::ConstructorClass tmp5 = {};
    REQUIRE(tmp5.checkState({0, 0, 0, Constructors::Constructor::Default}));

    // error: ‘class std::initializer_list<Constructors::ConstructorClass>’ has no member named ‘checkState’
    // auto tmp6 = {tmp1};
    // REQUIRE(tmp6.checkState({0, 0, 0, Constructors::Constructor::Default}));

    Constructors::ConstructorClass tmp7{270, 270};
    REQUIRE(!tmp7.checkState({0, 0, 0, Constructors::Constructor::Default}));
}

TEST_CASE("Unit test: testRvalueConsturctor")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass(1, 1);
    REQUIRE(tmp1.checkState({1, 1, 0, Constructors::Constructor::rvalueParam}));
    REQUIRE(!tmp1.checkState({1, 1, 0, Constructors::Constructor::lvalueParam}));
}

TEST_CASE("Unit test: testLvalueConsturctor")
{
    uint8_t order = 1;
    uint8_t data = 1;
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass(order, data);
    std::cout << tmp1 << std::endl;
    REQUIRE(!tmp1.checkState({order, data, 0, Constructors::Constructor::lvalueParam}));
    REQUIRE(tmp1.checkState({order, data, 0, Constructors::Constructor::rvalueParam}));
}

// Move tests
TEST_CASE("Unit test: testMoveConstructor1")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass(1, 10);
    REQUIRE(tmp1.checkState({1, 10, 0, Constructors::Constructor::rvalueParam}));
    Constructors::ConstructorClass tmp2 = std::move(tmp1);
    REQUIRE(tmp2.checkState({1, 10, 0, Constructors::Constructor::Move}));
    REQUIRE(tmp1.isFrogotten());
}

TEST_CASE("Unit test: testMoveConstructor2")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    REQUIRE(tmp1.checkState({0, 0, 0, Constructors::Constructor::Default}));
    Constructors::ConstructorClass tmp2 = (Constructors::ConstructorClass &&) tmp1;
    REQUIRE(tmp2.checkState({0, 0, 0, Constructors::Constructor::Move}));
    REQUIRE(tmp1.isFrogotten());
}

TEST_CASE("Unit test: testMoveConstructor3")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    REQUIRE(tmp1.checkState({0, 0, 0, Constructors::Constructor::Default}));
    Constructors::ConstructorClass tmp2 = std::move(tmp1 + 1);
    REQUIRE(tmp2.checkState({0, 1, 0, Constructors::Constructor::Move}));
    // REQUIRE(tmp1.isFrogotten()); // rvalue move i.e. not forgotten
}

TEST_CASE("Unit test: testMoveAssingConstructor")
{
    Constructors::ConstructorClass tmp0 = Constructors::ConstructorClass();
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    tmp0 = (Constructors::ConstructorClass &&) tmp1;
    REQUIRE(tmp0.checkState({0, 0, 0, Constructors::Constructor::MoveAssign}));
    REQUIRE(tmp1.isFrogotten());
}

// Copy tests
TEST_CASE("Unit test: testCopyConstructor1")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    Constructors::ConstructorClass tmp2 = tmp1;
    REQUIRE(tmp1.checkState({0, 0, 0, Constructors::Constructor::Default}));
    REQUIRE(tmp2.checkState({0, 0, 0, Constructors::Constructor::Copy}));
}

TEST_CASE("Unit test: testCopyConstructor2")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    Constructors::ConstructorClass tmp2 = Constructors::ConstructorClass(tmp1);
    REQUIRE(tmp1.checkState({0, 0, 0, Constructors::Constructor::Default}));
    REQUIRE(tmp2.checkState({0, 0, 0, Constructors::Constructor::Copy}));
}

TEST_CASE("Unit test: testCopyConstructor3")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    Constructors::ConstructorClass tmp2 = Constructors::ConstructorClass(tmp1);
    tmp1.setOrder(10);
    REQUIRE(tmp1.checkState({10, 0, 0, Constructors::Constructor::Default}));
    REQUIRE(tmp2.checkState({0, 0, 0, Constructors::Constructor::Copy}));
}

TEST_CASE("Unit test: testCopyConstructor4")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    Constructors::ConstructorClass tmp2(tmp1);

    REQUIRE(tmp1.checkState({0, 0, 0, Constructors::Constructor::Default}));
    REQUIRE(tmp2.checkState({0, 0, 0, Constructors::Constructor::Copy}));
}

TEST_CASE("Unit test: testCopyAssingConstructor")
{
    Constructors::ConstructorClass tmp0 = Constructors::ConstructorClass();
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    tmp0 = tmp1;

    REQUIRE(tmp0.checkState({0, 0, 0, Constructors::Constructor::CopyAssign}));

}

TEST_CASE("Unit test: testOperators")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();
    tmp1 = tmp1 + 10;
    REQUIRE(tmp1.checkState({0, 10, 0, Constructors::Constructor::MoveAssign}));

    Constructors::ConstructorClass tmp2 = Constructors::ConstructorClass();
    tmp2 = tmp1 - 1;
    REQUIRE(tmp2.checkState({0, 9, 0, Constructors::Constructor::MoveAssign}));

    Constructors::ConstructorClass tmp3 = Constructors::ConstructorClass();
    uint8_t plus = 10;
    tmp3 = tmp3 + plus;
    REQUIRE(tmp3.checkState({0, plus, 0, Constructors::Constructor::MoveAssign}));

}

TEST_CASE("Unit test: testWithVector")
{
    Constructors::ConstructorClass tmp1 = Constructors::ConstructorClass();


    std::vector<Constructors::ConstructorClass> v;
    v.reserve(10);
    v.push_back(tmp1);
    v.push_back(std::move(tmp1));

    REQUIRE(v[0].checkState({0, 0, 0, Constructors::Constructor::Copy}));
    REQUIRE(v[1].checkState({0, 0, 0, Constructors::Constructor::Move}));
}