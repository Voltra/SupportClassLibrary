#pragma once
#include <iostream>
#include <string>
#include <scl/stream/Stream.h>
#include <scl/stream/creators/stl_iterators.h>
#include <scl/stream/details/deprecation.h>

namespace scl{
	namespace stream{
		namespace creators{
			namespace details{
				/**
				 * Class that exposes custom logic for reading a file line by line
				 * for use with std::istream_iterator
				 * @tparam Char being the type of characters
				 * @tparam Traits being the traits of the type of characters
				 */
				template <class Char = char, class Traits = std::char_traits<Char>>
				struct ByLine : public std::basic_string<Char, Traits>{
					using stream_type = std::basic_istream<Char, Traits>;
					using string_type = std::basic_string<Char, Traits>;

					using std::basic_string<Char, Traits>::basic_string;

					inline friend stream_type& operator>>(stream_type& is, ByLine& line){
						std::getline(is, line);
						return is;
					}
				};

				/**
				 * istream iterator for reading line by line with conversion to std::basic_string
				 * @tparam Char being the type of characters
				 * @tparam Traits being the traits of the type of characters
				 */
				template <class Char = char, class Traits = std::char_traits<Char>>
				struct ByLineIterator : public std::istream_iterator<ByLine<Char, Traits>>{
					using std::istream_iterator<ByLine<Char, Traits>>::istream_iterator;

					using value_type = typename ByLine<Char, Traits>::string_type;
				};
			}

			/**
			 * Create a stream from a std::basic_istream
			 * @tparam Char being the type of characters to use
			 * @tparam Traits being the traits for these characters
			 * @param inputStream being the std::basic_istream to read from
			 * @return a Stream that uses the std::basic_istream as data source
			 */
			template <class Char = char, class Traits = std::char_traits<Char>>
			Stream<std::basic_string<Char, Traits>> streamFrom(std::basic_istream<Char, Traits>& inputStream){
				using iterator_t = details::ByLineIterator<Char, Traits>;
				return streamFrom(iterator_t{inputStream}, iterator_t{});
			}
		}
	}
}