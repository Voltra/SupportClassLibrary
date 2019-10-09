#pragma once

namespace scl{
	namespace utils{
		/**
		 * Class representing parameter/return argument placholders
		 * @warning Uses with std::unique_ptr and similar may result in dangling pointers
		 */
		struct Placeholder final{
			constexpr Placeholder() = default;
			constexpr Placeholder(const Placeholder&) = default;
			constexpr Placeholder(Placeholder&&) = default;
			Placeholder& operator=(const Placeholder&) = default;
			Placeholder& operator=(Placeholder&&) = default;
			~Placeholder() = default;

			/**
			 * Construct a placeholder from any kind of data
			 * @tparam T being the type of the data to omit
			 */
			template <class T>
			constexpr Placeholder(T&&){}

			/**
			 * Assign to a placeholder
			 * @tparam T being the type of the data to omit
			 * @return a reference to this placeholder
			 */
			template <class T>
			constexpr const Placeholder& operator=(T&&) const{ return *this; }
		};


		/**
		 * @namespace scl::utils::placeholder
		 * Namespace holding the constexpr global variable "_"
		 */
		namespace placeholder{
			constexpr scl::utils::Placeholder _ = {};
		}
	}
}