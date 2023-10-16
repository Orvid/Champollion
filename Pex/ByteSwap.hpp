#include <type_traits>
#include <cassert>
// These are backported from STL C++23

constexpr uint16_t _Byteswap_ushort(const uint16_t _Val) noexcept {
        return static_cast<unsigned short>((_Val << 8) | (_Val >> 8));
}

constexpr uint32_t _Byteswap_ulong(const uint32_t _Val) noexcept {
        return (_Val << 24) | ((_Val << 8) & 0x00FF'0000) | ((_Val >> 8) & 0x0000'FF00) | (_Val >> 24);
}

constexpr uint64_t _Byteswap_uint64(const uint64_t _Val) noexcept {
        return (_Val << 56) | ((_Val << 40) & 0x00FF'0000'0000'0000) | ((_Val << 24) & 0x0000'FF00'0000'0000)
             | ((_Val << 8) & 0x0000'00FF'0000'0000) | ((_Val >> 8) & 0x0000'0000'FF00'0000)
             | ((_Val >> 24) & 0x0000'0000'00FF'0000) | ((_Val >> 40) & 0x0000'0000'0000'FF00) | (_Val >> 56);
}

template <class... T>
constexpr bool _always_false_assert = false;

template <typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
constexpr Integer byteswap(const Integer _Val) noexcept {
    if constexpr (sizeof(Integer) == 1) {
        return _Val;
    } else if constexpr (sizeof(Integer) == 2) {
        return static_cast<Integer>(_Byteswap_ushort(static_cast<uint16_t>(_Val)));
    } else if constexpr (sizeof(Integer) == 4) {
        return static_cast<Integer>(_Byteswap_ulong(static_cast<uint32_t>(_Val)));
    } else if constexpr (sizeof(Integer) == 8) {
        return static_cast<Integer>(_Byteswap_uint64(static_cast<uint64_t>(_Val)));
    } else {
        static_assert(_always_false_assert<Integer>, "Unexpected integer size");
    }
}

float byteswap_float(const float _Val) noexcept {
    float retVal;
    const auto pVal = reinterpret_cast<const char*>(& _Val);
    const auto pRetVal = reinterpret_cast<char*>(& retVal);
    const std::size_t size = sizeof(float);
    assert(size == 4);
    for (std::size_t i = 0; i < size; i++)
    {
        pRetVal[size - 1 - i] = pVal[i];
    }
    return retVal;
}
