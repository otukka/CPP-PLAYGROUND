

/*
 * Bit operations
 * a=target variable
 * b=bit number to act upon 0-n
 * (!(!(a))) <= boolean
 * Modified from https://stackoverflow.com/questions/47981
 */
#define BIT_SET(a, b)              ((a) |= ((uint64_t)0x1 << (b)))
#define BIT_CLEAR(a, b)            ((a) &= ~((uint64_t)0x1 << (b)))
#define BIT_FLIP(a, b)             ((a) ^= ((uint64_t)0x1 << (b)))
#define BIT_CHECK(a, b)            (!(!((a) & ((uint64_t)0x1 << (b)))))
/*
 * Bitmask operations
 * Is there sizeof(x) >= sizeof(mask) constraint?
 */
#define BITMASK_SET(x, mask)       ((x) |= (mask))
#define BITMASK_CLEAR(x, mask)     ((x) &= (~(mask)))
#define BITMASK_FLIP(x, mask)      ((x) ^= (mask))
#define BITMASK_CHECK_ALL(x, mask) (!(~(x) & (mask)))
#define BITMASK_CHECK_ANY(x, mask) (!(!((x) & (mask))))