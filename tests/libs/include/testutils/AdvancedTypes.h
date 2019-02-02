#pragma once
#include <memory>
#include <mutex>

struct NonCopyable_t{
	std::unique_ptr<int> m{};
	int i{69};
	NonCopyable_t() = default;

	NonCopyable_t(const NonCopyable_t&) = delete;
	NonCopyable_t& operator=(const NonCopyable_t&) = delete;

	NonCopyable_t(NonCopyable_t&& o) = default;
	NonCopyable_t& operator=(NonCopyable_t&&) = default;
};

struct NonMovable_t{
	int i{42};
	std::mutex mut{};
	NonMovable_t() = default;

	NonMovable_t(NonMovable_t&&) = delete;
	NonMovable_t& operator=(NonMovable_t&&) = delete;

	NonMovable_t(const NonMovable_t&) = default;
	NonMovable_t& operator=(const NonMovable_t&) = default;
};

struct NonCopyableNonMovable_t{
	NonCopyable_t nc{};
	NonMovable_t nm{};
	int i{420};
	std::unique_ptr<int> p{};
	std::mutex mut{};

	NonCopyableNonMovable_t() = default;

	NonCopyableNonMovable_t(const NonCopyableNonMovable_t&) = delete;
	NonCopyableNonMovable_t(NonCopyableNonMovable_t&&) = delete;
	NonCopyableNonMovable_t& operator=(const NonCopyableNonMovable_t&) = delete;
	NonCopyableNonMovable_t& operator=(NonCopyableNonMovable_t&&) = delete;
};