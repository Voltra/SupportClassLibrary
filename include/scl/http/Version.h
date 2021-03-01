#pragma once
#include <scl/macros.h>
#include <scl/utils/toString.h>

namespace scl{
	namespace http{
		/**
		 * A pseudo enum class that describes the version of the HTTP Protocol
		 */
		class Version{
			public:
				/**
				 * @typedef version_type
				 * The string representation type
				 */
				using version_type = stringLiteral;

				/**
				 * @typedef integral_type
				 * The integer representation type
				 */
				using integral_type = unsigned char;

			protected:
				/**
				 * @var str
				 * The string representation of the version
				 */
				const version_type str;

				/**
				 * @var nb
				 * The integer representation of the version
				 */
				const integral_type nb;

				/**
				 * Constrct a version from its string and integer representation
				 * @param v being the string representation
				 * @param i being the integer representation
				 */
				explicit constexpr Version(const version_type& v, integral_type i) : str{v}, nb{i} {
				}

			public:
				/**
				 * Implicit conversion to string
				 * @return the string representation of this version
				 */
				constexpr explicit operator version_type() const{
					return this->str;
				}

				/**
				 * Implicit conversion to integer
				 * @return the integer representation of this version
				 */
				constexpr explicit operator integral_type() const{
					return this->nb;
				}

				constexpr bool operator==(const Version& other) const{
					return this->nb == other.nb;
				}

				constexpr bool operator<(const Version& other) const{
					return this->nb < other.nb;
				}

				constexpr bool operator<=(const Version& other) const{
					return *this < other || *this == other;
				}

				constexpr bool operator>(const Version& other) const{
					return !(*this <= other);
				}

				constexpr bool operator>=(const Version& other) const{
					return !(*this < other);
				}

				constexpr bool operator!=(const Version& other) const{
					return !(*this == other);
				}

			public:
				friend struct scl::utils::ToString<Version>;

#define VERSION(name) static const Version name
				VERSION(V1_0);
				VERSION(V1_1);
				VERSION(V2_0);
				VERSION(V3_0);
#undef VERSION
		};

#define DEF_VERSION(name, str, nb) const Version Version::name{str, nb}
		DEF_VERSION(V1_0, "1.0", 10);
		DEF_VERSION(V1_1, "1.1", 11);
		DEF_VERSION(V2_0, "2.0", 20);
		DEF_VERSION(V3_0, "3.0", 30);
#undef DEF_VERSION
	}

	namespace utils{
		template <>
		struct ToString<scl::http::Version, void>{
			static const std::string HTTP;
			std::string operator()(const scl::http::Version& v) const{
				return HTTP + v.str;
			}
		};

		const std::string ToString<scl::http::Version, void>::HTTP = "HTTP";
	}
}