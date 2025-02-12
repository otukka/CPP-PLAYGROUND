
template<typename T>
inline void BIT_SET(T& a, size_t b)
{
    a |= (0x1 << b);
}

template<typename T>
inline void BIT_CLEAR(T& a, size_t b)
{
    a &= ~(static_cast<T>(0x1) << static_cast<size_t>(b));
}

template<typename T>
inline void BIT_FLIP(T& a, size_t b)
{
    a ^= (0x1 << b);
}

template<typename T>
inline bool BIT_CHECK(const T& a, size_t b)
{
    return (!(!(a & (0x1 << b))));
}

template<typename T>
inline void BITMASK_SET(T& x, T mask)
{
    x |= mask;
}

template<typename T>
inline void BITMASK_CLEAR(T& x, T mask)
{
    x &= ~mask;
}

template<typename T>
inline void BITMASK_FLIP(T& x, T mask)
{
    x ^= mask;
}

template<typename T>
inline bool BITMASK_CHECK_ALL(const T& x, T mask)
{
    return (!(~x & mask));
}

template<typename T>
inline bool BITMASK_CHECK_ANY(const T& x, T mask)
{
    return (!(!((x) & (mask))));
}