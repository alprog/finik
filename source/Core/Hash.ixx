export module Hash;

import PrimitiveTypes;

export namespace Hash
{
    uint64 getMemoryHash(const byte* data, uint32 size)
    {
        uint64 hash = 0xcbf29ce484222325;
        uint64 prime = 0x100000001b3;

        for (uint32 i = 0; i < size; i++)
        {
            byte value = data[i];
            hash = hash ^ value;
            hash *= prime;
        }

        return hash;
    }

    template <typename T>
    uint64 getRawMemoryHash(const T* item)
    {
        return getMemoryHash(reinterpret_cast<const byte*>(item), sizeof(item));
    }
}
