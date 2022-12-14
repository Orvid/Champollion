#include <type_traits>
// These are backported from STL C++23

constexpr uint16_t _Byteswap_ushort(const uint16_t _Val) noexcept {
    if (std::_Is_constant_evaluated()) {
        return static_cast<unsigned short>((_Val << 8) | (_Val >> 8));
    } else {
        return _byteswap_ushort(_Val);
    }
}

constexpr uint32_t _Byteswap_ulong(const uint32_t _Val) noexcept {
    if (std::_Is_constant_evaluated()) {
        return (_Val << 24) | ((_Val << 8) & 0x00FF'0000) | ((_Val >> 8) & 0x0000'FF00) | (_Val >> 24);
    } else {
        return _byteswap_ulong(_Val);
    }
}

constexpr uint64_t _Byteswap_uint64(const uint64_t _Val) noexcept {
    if (std::_Is_constant_evaluated()) {
        return (_Val << 56) | ((_Val << 40) & 0x00FF'0000'0000'0000) | ((_Val << 24) & 0x0000'FF00'0000'0000)
             | ((_Val << 8) & 0x0000'00FF'0000'0000) | ((_Val >> 8) & 0x0000'0000'FF00'0000)
             | ((_Val >> 24) & 0x0000'0000'00FF'0000) | ((_Val >> 40) & 0x0000'0000'0000'FF00) | (_Val >> 56);
    } else {
        return _byteswap_uint64(_Val);
    }
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
