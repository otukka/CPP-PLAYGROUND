

#include "Ringbuffer.h"

static ringbuffer_t m_ringbuffer;

static uint32_t returnBuffer[RINGBUFFER_LENGTH] = { 0 };

void ringbufferInit(void)
{
    memset(&m_ringbuffer, 0x00, sizeof(ringbuffer_t));
    memset(&returnBuffer, 0x00, sizeof(returnBuffer));
}

void addValue(uint32_t a_value)
{
    m_ringbuffer.buffer[m_ringbuffer.end & (RINGBUFFER_LENGTH - 1)] = a_value;
    if (isFull())
    {
        m_ringbuffer.start &= ((RINGBUFFER_LENGTH * 2) - 1);
        m_ringbuffer.start++;
    }
    m_ringbuffer.end &= ((RINGBUFFER_LENGTH * 2) - 1);
    m_ringbuffer.end++;
}

uint32_t* getValues(uint32_t a_length)
{
    for (size_t i = 0; i < a_length; i++)
    {
        returnBuffer[i] = m_ringbuffer.buffer[m_ringbuffer.start & (RINGBUFFER_LENGTH - 1)];
        m_ringbuffer.start &= ((RINGBUFFER_LENGTH * 2) - 1);
        m_ringbuffer.start++;
    }
    return &returnBuffer[0];
}

uint32_t getEnd(void)
{
    return m_ringbuffer.end & (RINGBUFFER_LENGTH - 1);
}

uint32_t getStart(void)
{
    return m_ringbuffer.start & (RINGBUFFER_LENGTH - 1);
}

bool isEmpty(void)
{
    return (m_ringbuffer.start == m_ringbuffer.end) ? true : false;
}

bool isFull(void)
{
    return (getSize() == RINGBUFFER_LENGTH) ? true : false;
}

uint32_t getSize(void)
{
    return (((m_ringbuffer.end) - (m_ringbuffer.start)) & ((RINGBUFFER_LENGTH * 2) - 1));
}

bool isValid(void)
{
    return ((m_ringbuffer.end > (RINGBUFFER_LENGTH * 2)) || (m_ringbuffer.start > (RINGBUFFER_LENGTH * 2))) ? false
                                                                                                            : true;
}