export module primitive_types;

constexpr int BIT_COUNT = 8;

export using int8 = signed char;
export using int16 = signed short;
export using int32 = signed int;
export using int64 = signed long long;

export using uint8 = unsigned char;
export using uint16 = unsigned short;
export using uint32 = unsigned int;
export using uint64 = unsigned long long;

export using byte = uint8;

static_assert(sizeof(int8) * BIT_COUNT == 8);
static_assert(sizeof(int16) * BIT_COUNT == 16);
static_assert(sizeof(int32) * BIT_COUNT == 32);
static_assert(sizeof(int64) * BIT_COUNT == 64);