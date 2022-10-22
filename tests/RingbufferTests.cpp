
#include <catch2/catch_all.hpp>

#include "Ringbuffer.h"

TEST_CASE("Distinguish empty and full")
{
    ringbufferInit();
    CHECK(isEmpty() == true);
    CHECK(isFull() == false);
    for (size_t i = 0; i < RINGBUFFER_LENGTH - 1; i++)
    {
        addValue(i);
        CHECK(isFull() == false);
        CHECK(isEmpty() == false);
    }

    for (size_t i = 0; i < RINGBUFFER_LENGTH * 10; i++)
    {
        addValue(i);
        CHECK(isFull() == true);
        CHECK(isEmpty() == false);
    }
}

TEST_CASE("Fill to full then empty")
{
    ringbufferInit();
 
    CHECK(isEmpty() == true);
    CHECK(isFull() == false);

    for (size_t i = 0; i < RINGBUFFER_LENGTH; i++)
    {
        addValue(i);
    }

    uint32_t* buffer = getValues(RINGBUFFER_LENGTH);

    CHECK(isEmpty() == true);
    CHECK(isFull() == false);
}

TEST_CASE("Size never too large")
{
    ringbufferInit();
    CHECK(isEmpty() == true);
    for (size_t i = 0; i < 10000; i++)
    {

        addValue(i);
        if (i >= (RINGBUFFER_LENGTH - 1))
        {
            CHECK(getSize() == RINGBUFFER_LENGTH);
            CHECK(isFull() == true);
        }
        else
        {
            CHECK(isFull() == false);
        }
        INFO("Iteration: " << i);
        CHECK(isValid() == true);
    }
}

TEST_CASE("Basic fill ringbuffer")
{
    ringbufferInit();
    for (size_t i = 0; i < (RINGBUFFER_LENGTH - 1); i++)
    {

        addValue(i);
        INFO("Iteration: " << i);
        CHECK(getStart() == 0);
        CHECK(getEnd() == (i + 1));
        CHECK(getSize() == (i + 1));
    }

    addValue((RINGBUFFER_LENGTH - 1));

    CHECK(getStart() == 0);
    CHECK(getEnd() == 0);
    CHECK(getSize() == RINGBUFFER_LENGTH);

    uint32_t* buffer = getValues(RINGBUFFER_LENGTH);
    for (size_t i = 0; i < RINGBUFFER_LENGTH; i++)
    {
        CHECK(buffer[i] == i);
    }
}

TEST_CASE("More complex fill ringbuffer")
{
    ringbufferInit();
    for (size_t i = 0; i < RINGBUFFER_LENGTH; i++)
    {
        addValue(i);
    }

    uint32_t* buffer = getValues((RINGBUFFER_LENGTH / 2));
    for (size_t i = 0; i < (RINGBUFFER_LENGTH / 2); i++)
    {
        CHECK(buffer[i] == i);
    }

    CHECK(getStart() == (RINGBUFFER_LENGTH / 2));
    CHECK(getEnd() == 0);

    for (size_t i = 0; i < (RINGBUFFER_LENGTH / 2); i++)
    {
        addValue(i);
    }

    CHECK(getStart() == (RINGBUFFER_LENGTH / 2));
    CHECK(getEnd() == (RINGBUFFER_LENGTH / 2));
    CHECK(getSize() == RINGBUFFER_LENGTH);

    for (size_t i = 0; i < (RINGBUFFER_LENGTH / 2); i++)
    {
        addValue(i);
    }

    CHECK(getStart() == 0);
    CHECK(getEnd() == 0);
    CHECK(getSize() == RINGBUFFER_LENGTH);

    buffer = getValues(1);
    CHECK(getStart() == 1);
    CHECK(getEnd() == 0);
    CHECK(getSize() == (RINGBUFFER_LENGTH - 1));
}

TEST_CASE("Read at center")
{
    ringbufferInit();
    for (size_t i = 0; i < RINGBUFFER_LENGTH; i++)
    {
        addValue(i);
    }

    CHECK(getStart() == 0);
    CHECK(getEnd() == 0);

    uint32_t* buffer = getValues((RINGBUFFER_LENGTH / 2));
    for (size_t i = 0; i < (RINGBUFFER_LENGTH / 2); i++)
    {
        CHECK(buffer[i] == i);
    }

    CHECK(getStart() == (RINGBUFFER_LENGTH / 2));
    CHECK(getEnd() == 0);

    for (size_t i = RINGBUFFER_LENGTH; i < (RINGBUFFER_LENGTH * 3 / 2); i++)
    {
        addValue(i);
    }

    CHECK(getStart() == (RINGBUFFER_LENGTH / 2));
    CHECK(getEnd() == (RINGBUFFER_LENGTH / 2));

    buffer = getValues(RINGBUFFER_LENGTH);

    CHECK(getStart() == (RINGBUFFER_LENGTH / 2));
    CHECK(getEnd() == (RINGBUFFER_LENGTH / 2));

    for (size_t i = RINGBUFFER_LENGTH; i < (RINGBUFFER_LENGTH * 2); i++)
    {
        CHECK(buffer[i - RINGBUFFER_LENGTH] == i - (RINGBUFFER_LENGTH / 2));
    }
}

TEST_CASE("Fill empty one by one")
{
    ringbufferInit();
    for (size_t i = 0; i < RINGBUFFER_LENGTH; i++)
    {
        addValue(i);
    }

    for (size_t i = 0; i < RINGBUFFER_LENGTH; i++)
    {
        CHECK(getStart() == i);
        uint32_t* buffer = getValues(1);
        CHECK(buffer[0] == i);
    }
}