#include <doctest/doctest.h>
#include <scl/scl.hpp>
#include <scl/version.h>

#include <string>

TEST_CASE("SupportClassLibrary") {
  CHECK(true);
}

TEST_CASE("SupportClassLibrary version") {
  static_assert(std::string_view(SupportClassLibrary_VERSION) == std::string_view("1.0.0"));
  CHECK(std::string(SupportClassLibrary_VERSION) == std::string("1.0.0"));
}
