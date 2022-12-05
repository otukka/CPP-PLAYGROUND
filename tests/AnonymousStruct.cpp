

#include <catch2/catch_all.hpp>

#include <vector>
#include <unordered_map>

namespace NewImpl
{
enum types
{
    C,
    A,
    B,
    UNKNOWN
};
}

namespace OldImpl
{
enum types
{
    A,
    B,
    C,
    D,
    E,
    F,
};
}

namespace
{
struct conversionStruct
{
    NewImpl::types newImpl;
    OldImpl::types oldImpl;
};

const std::vector<conversionStruct> structConverter = {
    { NewImpl::types::C, OldImpl::types::A },
    { NewImpl::types::A, OldImpl::types::B },
    { NewImpl::types::B, OldImpl::types::C },
};

const std::unordered_map<OldImpl::types, NewImpl::types> mapConverter = {
    { OldImpl::types::C, NewImpl::types::A },
    { OldImpl::types::A, NewImpl::types::B },
    { OldImpl::types::B, NewImpl::types::C },
};

}

NewImpl::types convertWithVector(OldImpl::types old)
{
    NewImpl::types ret = NewImpl::types::UNKNOWN;
    for (auto var : structConverter)
    {
        if (var.oldImpl == old)
        {
            ret = var.newImpl;
        }
    }
    return ret;
}

NewImpl::types convertWithMap(OldImpl::types old)
{
    NewImpl::types ret = NewImpl::types::UNKNOWN;

    auto it = mapConverter.find(old);
    if (it != mapConverter.end())
    {
        ret = it->second;
    }
    return ret;
}

TEST_CASE("Conversion test with vector container")
{

    CHECK(convertWithVector(OldImpl::types::A) == NewImpl::types::C);
    CHECK(convertWithVector(OldImpl::types::B) == NewImpl::types::A);
    CHECK(convertWithVector(OldImpl::types::C) == NewImpl::types::B);

    CHECK(convertWithVector(OldImpl::types::D) == NewImpl::types::UNKNOWN);
    CHECK(convertWithVector(OldImpl::types::E) == NewImpl::types::UNKNOWN);
    CHECK(convertWithVector(OldImpl::types::F) == NewImpl::types::UNKNOWN);

    CHECK_FALSE(convertWithVector(OldImpl::types::A) == NewImpl::types::A);
    CHECK_FALSE(convertWithVector(OldImpl::types::B) == NewImpl::types::B);
    CHECK_FALSE(convertWithVector(OldImpl::types::C) == NewImpl::types::C);
}

TEST_CASE("Conversion test with map container")
{

    CHECK(convertWithMap(OldImpl::types::A) == NewImpl::types::B);
    CHECK(convertWithMap(OldImpl::types::B) == NewImpl::types::C);
    CHECK(convertWithMap(OldImpl::types::C) == NewImpl::types::A);

    CHECK(convertWithMap(OldImpl::types::D) == NewImpl::types::UNKNOWN);
    CHECK(convertWithMap(OldImpl::types::E) == NewImpl::types::UNKNOWN);
    CHECK(convertWithMap(OldImpl::types::F) == NewImpl::types::UNKNOWN);

    CHECK_FALSE(convertWithMap(OldImpl::types::A) == NewImpl::types::A);
    CHECK_FALSE(convertWithMap(OldImpl::types::B) == NewImpl::types::B);
    CHECK_FALSE(convertWithMap(OldImpl::types::C) == NewImpl::types::C);
}