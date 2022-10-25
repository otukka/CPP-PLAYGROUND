#include <catch2/catch_all.hpp>
#include "BitOperations.hpp"

TEST_CASE("Testing macros: check assumptions")
{
    CHECK(((!!0) == 0));
    CHECK((((!(!(0)))) == 0));

    for (uint8_t i = 1; i < UINT8_MAX; i++)
    {
        CHECK(((!!i) == 1));
    }

    CHECK(1);           // 1 = true
    CHECK_FALSE(0);     // 0 = false
    CHECK(true);        // 1 = true
    CHECK_FALSE(false); // 0 = false
}

TEST_CASE("Testing macros: bit: sequence", "[bit]")
{

    uint8_t test;

    for (size_t i = 0; i < 7; i++)
    {
        for (uint8_t j = 0x00; j < 0xFF; j++)
        {
            uint8_t tmp = j; /* copy value because modification are inplace and loop might stuck */
            BIT_SET(tmp, i);
            CHECK(BIT_CHECK(tmp, i));
            BIT_CLEAR(tmp, i);
            CHECK_FALSE(BIT_CHECK(tmp, i));
            BIT_FLIP(tmp, i);
            CHECK(BIT_CHECK(tmp, i));
            BIT_FLIP(tmp, i);
            CHECK_FALSE(BIT_CHECK(tmp, i));
        }
    }
}

TEST_CASE("Testing macros: bitmask: sequence", "[bitmask]")
{
    uint8_t test;

    /* All same */
    for (uint8_t i = 0x01; i < 0xFF; i++)
    {
        uint8_t tmp = i;

        BITMASK_SET(tmp, i);
        CHECK(BITMASK_CHECK_ANY(tmp, i));
        CHECK(BITMASK_CHECK_ALL(tmp, i));

        BITMASK_CLEAR(tmp, i);
        CHECK(tmp == 0x00);

        BITMASK_SET(tmp, i);
        BITMASK_FLIP(tmp, i);
        CHECK(tmp == 0x00);

        BITMASK_FLIP(tmp, i);
        CHECK(BITMASK_CHECK_ANY(tmp, i));
        CHECK(BITMASK_CHECK_ALL(tmp, i));
    }
}

TEST_CASE("Testing macros: bitmask: same to both fields", "[bitmask]")
{
    uint8_t test;
    /* All same */
    for (uint8_t i = 0x01; i < 0xFF; i++)
    {
        CHECK(BITMASK_CHECK_ANY(i, i));
        CHECK(BITMASK_CHECK_ALL(i, i));
        uint8_t copy = i;
        CHECK_FALSE(BITMASK_CHECK_ANY((~copy), copy));
        CHECK_FALSE(BITMASK_CHECK_ALL((~copy), copy));
    }

    /* All zeros */
    test = 0x00;
    for (uint8_t i = 0x01; i < 0xFF; i++)
    {
        CHECK_FALSE(BITMASK_CHECK_ANY(test, i));
        CHECK_FALSE(BITMASK_CHECK_ALL(test, i));
    }
}

TEST_CASE("Testing macros: bitmask: all ones", "[bitmask]")
{
    uint8_t test;
    /* All ones*/
    test = 0xFF;
    for (uint8_t i = 0x01; i < 0xFF; i++)
    {
        CHECK(BITMASK_CHECK_ANY(test, i));
        CHECK(BITMASK_CHECK_ALL(test, i));
    }
}

TEST_CASE("Testing macros: bitmask: first byte ones", "[bitmask]")
{
    uint8_t test;
    /* First byte ones*/
    test = 0xF0;
    for (uint8_t i = 0xF1; i < 0xFF; i++)
    {
        CHECK_FALSE(BITMASK_CHECK_ALL(test, i));
        CHECK(BITMASK_CHECK_ANY(test, i));
    }
}

TEST_CASE("Testing macros: bitmask: second byte ones", "[bitmask]")
{
    uint8_t test;
    /* Second byte ones*/
    test = 0x0F;
    for (uint8_t i = 0xF1; i < 0xFF; i++)
    {
        CHECK_FALSE(BITMASK_CHECK_ALL(test, i));
        CHECK(BITMASK_CHECK_ANY(test, i));
    }
}