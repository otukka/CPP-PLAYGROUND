#include <catch2/catch_all.hpp>

#include <iostream>
#include <iomanip>
#include <bitset>
#include <array>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

/**
 * @brief Convert number to twos complement version or other way around.
 *
 * @tparam A
 * @tparam B
 * @param from
 * @param to
 * @param toBits
 *
 * Example you want to store signed value e.g. -12.3 in deci-format(-123) as signed value.
 */
template <class A, class B> void twosCompelement(const A& from, B& to, u32 toBits)
{
    u32 shifter = sizeof(B) * 8 - toBits;
    to = static_cast<B>(from << shifter) / (1 << shifter);
}

TEST_CASE("Bitset testing")
{
    CHECK(std::bitset<4> { 0x3 } == 0x3);
    CHECK_FALSE(std::bitset<4> { 0x3 } == 0xA);

    CHECK(std::bitset<4> { "0011" } == 0x3);
    CHECK(std::bitset<4> { "0011" } == 0b0011);
}

TEST_CASE("static_cast u16 to u8")
{
    std::array<u16, 8> input { 0, 1, 2, 3, 254, 255, 256, 257 };
    std::array<u8, 8> output { 0, 1, 2, 3, 254, 255, 0, 1 };

    for (size_t i = 0; i < input.size(); i++)
    {
        u16 b = static_cast<u8>(input.at(i));
        CHECK(b == output.at(i));
    }
}

TEST_CASE("signed to unsigned")
{
    std::array<i16, 8> input { 0, 1, 2, 3, -4, -3, -2, -1 };
    std::array<u16, 8> output { 0, 1, 2, 3, 4, 5, 6, 7 };

    for (size_t i = 0; i < input.size(); i++)
    {
        u16 b;
        twosCompelement(input.at(i), b, 3);
        CHECK(b == output.at(i));
    }
}

TEST_CASE("unsigned to signed")
{

    std::array<u16, 8> input { 0, 1, 2, 3, 4, 5, 6, 7 };
    std::array<i16, 8> output { 0, 1, 2, 3, -4, -3, -2, -1 };

    for (size_t i = 0; i < input.size(); i++)
    {
        i16 b;
        twosCompelement(input.at(i), b, 3);
        CHECK(b == output.at(i));
    }
}

TEST_CASE("u to i to u")
{
    std::array<u16, 8> input { 0, 1, 2, 3, 4, 5, 6, 7 };
    for (size_t i = 0; i < input.size(); i++)
    {
        i16 b;
        twosCompelement(input.at(i), b, 3);
        u16 c;
        twosCompelement(b, c, 3);
        CHECK(c == input.at(i));
    }
}

TEST_CASE("i to u to i")
{
    std::array<i16, 8> input { 0, 1, 2, 3, -4, -3, -2, -1 };
    for (size_t i = 0; i < input.size(); i++)
    {
        u16 b;
        twosCompelement(input.at(i), b, 3);
        i16 c;
        twosCompelement(b, c, 3);
        CHECK(c == input.at(i));
    }
}

TEST_CASE("8-bit signed 16-bit unsigned")
{
    std::array<i8, 3> input { 0, 1, -1 };
    std::array<u16, 3> output { 0, 1, 65535 };

    for (size_t i = 0; i < input.size(); i++)
    {
        u16 b;
        twosCompelement(input.at(i), b, 16);
        CHECK(b == output.at(i));
    }
}

TEST_CASE("16-bit signed 8-bit unsigned")
{
    std::array<i16, 3> input { 0, 1, -1 };
    std::array<u8, 3> output { 0, 1, 255 };

    for (size_t i = 0; i < input.size(); i++)
    {
        u8 b;
        twosCompelement(input.at(i), b, 8);
        CHECK(b == output.at(i));
    }
}