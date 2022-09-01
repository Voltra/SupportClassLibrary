#pragma once
#include <exception>
#include <iterator>
#include <type_traits>

namespace scl {
    namespace utils {
        namespace details {
            template <class Char, class Traits = std::char_traits<Char>>
            class BasicStringView {
                public:
                    using traits_type = Traits;
                    using Traits_type = traits_type;
                    using value_type = Char;
                    using pointer = value_type*;
                    using const_pointer = const value_type*;
                    using reference = value_type&;
                    using const_reference = const value_type&;
                    using const_iterator = const_pointer;
                    using iterator = const_iterator;
                    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
                    using reverse_iterator = const_reverse_iterator;
                    using size_type = std::size_t;
                    using difference_type = std::ptrdiff_t;

                    static constexpr size_type npos = size_type(-1);

                protected:
                    const_pointer start = nullptr;
                    size_type length_ = 0;

                public:
                    constexpr BasicStringView() noexcept = default;
                    constexpr BasicStringView(const BasicStringView&) noexcept = default;
                    constexpr BasicStringView(std::nullptr_t) noexcept = delete;
                    constexpr BasicStringView(const_pointer str, size_type len) noexcept
                        : start{str}, length_{len} {}
                    constexpr BasicStringView(const_pointer begin, const_pointer end) noexcept
                        : start{begin}, length_{std::distance(begin, end)} {}

                    template <size_type N>
                    constexpr BasicStringView(const value_type (&arr)[N]) noexcept
                        : start{arr}, length_{N} {}

                    template <class Container>
                    constexpr BasicStringView(Container&& container) noexcept
                        : start{std::begin(container)}, length_{std::size(container)} {}

                    constexpr BasicStringView& operator=(const BasicStringView&) noexcept = default;

                    SCL_NODISCARD inline constexpr const_iterator begin() const noexcept {
                        return start;
                    }

                    SCL_NODISCARD inline constexpr const_iterator end() const noexcept {
                        return start + length_;
                    }

                    SCL_NODISCARD inline constexpr const_iterator cbegin() const noexcept {
                        return begin();
                    }

                    SCL_NODISCARD inline constexpr const_iterator cend() const noexcept {
                        return end();
                    }

                    SCL_NODISCARD inline constexpr const_reverse_iterator rbegin() const noexcept {
                        return start + length_;
                    }

                    SCL_NODISCARD inline constexpr const_reverse_iterator rend() const noexcept {
                        return start;
                    }

                    SCL_NODISCARD inline constexpr const_reverse_iterator crbegin() const noexcept {
                        return rbegin();
                    }

                    inline constexpr const_reverse_iterator crend() const noexcept {
                        return rend();
                    }

                    SCL_NODISCARD inline constexpr size_type size() const noexcept {
                        return length_;
                    }

                    SCL_NODISCARD inline constexpr size_type length() const noexcept {
                        return size();
                    }

                    SCL_NODISCARD inline constexpr size_type max_size() const noexcept {
                        return size();
                    }

                    SCL_NODISCARD inline constexpr bool empty() const noexcept {
                        return size() == 0;
                    }

                    SCL_NODISCARD inline const_reference operator[](
                        size_type index) const noexcept {
                        return start[index];
                    }

                    SCL_NODISCARD inline const_reference at(size_type index) const {
                        if (index < 0) {
                            throw std::out_of_range("Index < 0 in scl::utils::BasicStringView");
                        }

                        if (index >= length_) {
                            throw std::out_of_range(
                                "Index past the end in scl::utils::BasicStringView");
                        }

                        return start[index];
                    }

                    SCL_NODISCARD inline const_reference front() const noexcept {
                        return operator[](0);
                    }

                    SCL_NODISCARD inline const_reference back() const noexcept {
                        return operator[](size() - 1);
                    }

                    SCL_NODISCARD inline const_pointer data() const noexcept { return start; }
            };

            template <class Char, class Traits = std::char_traits<Char>>
            class NullTerminatedBasicStringPtr {
                public:
                    using string_view = BasicStringView<Char, Traits>;
                    using const_pointer = typename string_view::const_pointer;
                    using const_reference = typename string_view::const_reference;
                    using size_type = typename string_view::size_type;

                    constexpr NullTerminatedBasicStringPtr(string_view sv)
                        : ptr{sv.data()}, begin_{sv.begin()}, end_{sv.end()} {}

                    constexpr NullTerminatedBasicStringPtr() = delete;
                    constexpr NullTerminatedBasicStringPtr(
                        const NullTerminatedBasicStringPtr&) noexcept = default;
                    constexpr NullTerminatedBasicStringPtr(
                        NullTerminatedBasicStringPtr&&) noexcept = default;
                    constexpr NullTerminatedBasicStringPtr& operator=(
                        const NullTerminatedBasicStringPtr&) noexcept = default;
                    constexpr NullTerminatedBasicStringPtr& operator=(
                        NullTerminatedBasicStringPtr&&) noexcept = default;

                    constexpr const_reference operator*() noexcept {
                        return ptr == end_ ? '\0' : *ptr;
                    }

                    constexpr NullTerminatedBasicStringPtr& operator++() noexcept {
                        if (ptr != end_) {
                            ++ptr;
                        }

                        return *this;
                    }

                    constexpr NullTerminatedBasicStringPtr operator++(int) noexcept {
                        auto self = *this;
                        ++(*this);
                        return self;
                    }

                    constexpr NullTerminatedBasicStringPtr& operator--() noexcept {
                        if (ptr != begin_) {
                            --ptr;
                        }

                        return *this;
                    }

                    constexpr NullTerminatedBasicStringPtr operator--(int) noexcept {
                        auto self = *this;
                        --(*this);
                        return self;
                    }

                protected:
                    const_pointer ptr, begin_, end_;
            };
        }  // namespace details

        using StringView = details::BasicStringView<char>;
        using WStringView = details::BasicStringView<wchar_t>;
        using NTStringViewPtr = details::NullTerminatedBasicStringPtr<char>;
        using NTWStringViewPtr = details::NullTerminatedBasicStringPtr<wchar_t>;
    }  // namespace utils
}  // namespace scl
