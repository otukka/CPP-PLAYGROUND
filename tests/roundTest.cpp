

#include <catch2/catch_all.hpp>
#include <math.h>

uint32_t rounder(uint32_t val, uint32_t pres)
{
    return ((val / pres) * pres);
}

TEST_CASE("round numbers")
{
    CHECK(rounder(100, 10) == 100);
    CHECK_FALSE(rounder(100, 10) == 10);

    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 0))) == 123456789);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 1))) == 123456780);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 2))) == 123456700);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 3))) == 123456000);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 4))) == 123450000);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 5))) == 123400000);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 6))) == 123000000);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 7))) == 120000000);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 8))) == 100000000);
    CHECK(rounder(123456789, static_cast<uint32_t>(pow(10, 9))) == 000000000);
}

TEST_CASE("nanosecond int")
{
    uint32_t nanosecond = pow(10, 9);

    CHECK(rounder(static_cast<uint32_t>(pow(10, 0)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 1)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 2)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 3)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 4)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 5)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 6)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 7)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 8)), nanosecond) == 0);
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9)), nanosecond) == static_cast<uint32_t>(pow(10, 9)));

    // Second to nanoseconds
    CHECK(static_cast<uint32_t>(pow(10, 9) == static_cast<uint32_t>(1000000000)));

    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 1), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 2), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 3), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 4), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 5), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 6), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 7), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 8), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 9), nanosecond) == static_cast<uint32_t>(pow(10, 9)));

    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 1), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 12), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 123), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 1234), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 12345), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 123456), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 1234567), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 12345678), nanosecond) == static_cast<uint32_t>(pow(10, 9)));
    CHECK(rounder(static_cast<uint32_t>(pow(10, 9) + 123456789), nanosecond) == static_cast<uint32_t>(pow(10, 9)));

    CHECK_FALSE(rounder(static_cast<uint32_t>(pow(10, 9) + 1234567890), nanosecond)
                == static_cast<uint32_t>(pow(10, 9)));
}