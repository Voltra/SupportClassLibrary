#pragma once
#include <algorithm>
#include <array>

#include "../meta/type_aliases/stl.h"
#include "../meta/type_queries/stl.h"

#ifdef SCL_CPP17
    #include <cstddef>
#endif

namespace scl {
    namespace alias {
#ifdef SCL_CPP17
	using byte = std::byte;
#else
	enum class byte : unsigned char {};

	template <typename Integral>
	constexpr scl::meta::enable_if_t<scl::meta::is_integral<Integral>(), Integral> to_integer(
	    byte b) noexcept {
	    return Integral{b};
	}

	template <typename Integral>
	constexpr scl::meta::enable_if_t<scl::meta::is_integral<Integral>(), byte> to_byte(
	    Integral integral) noexcept {
	    return static_cast<byte>(integral);
	}

	template <typename Integral>
	constexpr scl::meta::enable_if_t<scl::meta::is_integral<Integral>(), byte> as_byte(
	    Integral integral) noexcept {
	    return to_byte(integral);
	}

	template <typename Integral>
	constexpr scl::meta::enable_if_t<scl::meta::is_integral<Integral>(), byte> operator<<(
	    byte lhs, Integral rhs) noexcept {
	    return static_cast<unsigned int>(lhs) << rhs;
	}

	template <typename Integral>
	constexpr scl::meta::enable_if_t<scl::meta::is_integral<Integral>(), byte> operator>>(
	    byte lhs, Integral rhs) noexcept {
	    return static_cast<unsigned int>(lhs) >> rhs;
	}

	constexpr byte operator|(byte lhs, byte rhs) noexcept {
	    return static_cast<byte>(static_cast<unsigned int>(lhs)
				     | static_cast<unsigned int>(rhs));
	}

	constexpr byte operator&(byte lhs, byte rhs) noexcept {
	    return static_cast<byte>(static_cast<unsigned int>(lhs)
				     & static_cast<unsigned int>(rhs));
	}

	constexpr byte operator^(byte lhs, byte rhs) noexcept {
	    return static_cast<byte>(static_cast<unsigned int>(lhs)
				     ^ static_cast<unsigned int>(rhs));
	}

	constexpr byte operator~(byte b) noexcept {
	    return static_cast<byte>(~(static_cast<unsigned int>(b)));
	}

	template <typename Integral>
	constexpr scl::meta::enable_if_t<scl::meta::is_integral<Integral>(), byte&> operator<<=(
	    byte& lhs, Integral rhs) noexcept {
	    return lhs = lhs << rhs;
	}

	template <typename Integral>
	constexpr scl::meta::enable_if_t<scl::meta::is_integral<Integral>(), byte&> operator>>=(
	    byte& lhs, Integral rhs) noexcept {
	    return lhs = lhs >> rhs;
	}

	constexpr byte& operator|=(byte& lhs, byte rhs) noexcept {
	    return lhs = lhs | rhs;
	}
	constexpr byte& operator&=(byte& lhs, byte rhs) noexcept {
	    return lhs = lhs & rhs;
	}
	constexpr byte& operator^=(byte& lhs, byte rhs) noexcept {
	    return lhs = lhs ^ rhs;
	}
#endif

	constexpr byte* ptr_as_bytes(void* ptr) {
	    return static_cast<byte*>(ptr);
	}

	template <typename Integral>
	constexpr std::array<byte, sizeof(Integral)> to_bytes(const Integral& value) {
	    byte* repr = ptr_as_bytes(&value);
	    std::array<byte, sizeof(Integral)> ret{};
	    std::copy(repr, repr + sizeof(Integral), ret.begin());
	    return ret;
	}
    }  // namespace alias
}  // namespace scl
