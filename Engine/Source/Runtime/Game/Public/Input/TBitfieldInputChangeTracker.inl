// Copyright 2020-2021 Aumoa.lib. All right reserved.

template<class TBitfield>
TBitfieldInputChangeTracker<TBitfield>::TBitfieldInputChangeTracker()
{

}

template<class TBitfield>
void TBitfieldInputChangeTracker<TBitfield>::Update(const Packer32Bits& newState)
{
    auto currPtr = reinterpret_cast<const uint32*>(&newState);
    auto prevPtr = reinterpret_cast<const uint32*>(&last);
    auto releasedPtr = reinterpret_cast<uint32*>(&released);
    auto pressedPtr = reinterpret_cast<uint32*>(&pressed);

    // Fast-check changes of state using 32-bit unit.
    for (size_t i = 0; i < (sizeof(TBitfield) / 4); ++i)
    {
        *pressedPtr = *currPtr & ~(*prevPtr);
        *releasedPtr = ~(*currPtr) & *prevPtr;

        ++currPtr;
        ++prevPtr;
        ++releasedPtr;
        ++pressedPtr;
    }

    // Slow-check changes of state using remainder.
    if (size_t rem = sizeof(TBitfield) % 32; rem != 0)
    {
        int32 curr = *currPtr;
        int32 prev = *prevPtr;

        *pressedPtr = curr & ~prev;
        *releasedPtr = ~curr & prev;
    }

    last = newState;
}

template<class TBitfield>
void TBitfieldInputChangeTracker<TBitfield>::Reset()
{
    memset(&pressed, 0, sizeof(pressed));
    memset(&released, 0, sizeof(released));
}

template<class TBitfield>
bool TBitfieldInputChangeTracker<TBitfield>::IsPressedByBitIndex(uint32 bitIndex) const
{
#if defined(_DEBUG)
    if (bitIndex > sizeof(TBitfield) * 8)
    {
        throw Exception(String::Format(L"bitIndex is over than sizeof bits of TBitfield structure. bitIndex: {0}, size: {1}", bitIndex, sizeof(TBitfield) * 8));
        return false;
    }
#endif

    auto ptr = reinterpret_cast<const uint32*>(&pressed);
    unsigned int bf = 1u << bitIndex;
    return (ptr[(bitIndex >> 5)] & bf) != 0;
}

template<class TBitfield>
bool TBitfieldInputChangeTracker<TBitfield>::IsReleasedByBitIndex(uint32 bitIndex) const
{
#if defined(_DEBUG)
    if (bitIndex > sizeof(TBitfield) * 8)
    {
        throw Exception(String::Format(L"bitIndex is over than sizeof bits of TBitfield structure. bitIndex: {0}, size: {1}", bitIndex, sizeof(TBitfield) * 8));
        return false;
    }
#endif

    auto ptr = reinterpret_cast<const uint32*>(&released);
    unsigned int bf = 1u << bitIndex;
    return (ptr[(bitIndex >> 5)] & bf) == 0;
}