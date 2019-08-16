#pragma once

#include <scl/stream/details/iterator/BaseStreamIterator.h>
#include <scl/macros.h>

namespace scl{
	namespace stream{
		/**
		 * Class representing a stream of data
		 * @tparam T being the type of data
		 */
		template <class T>
		class Stream{
				static_assert(!META::is_void<T>(), "Cannot make a Stream<void>");
			public:
				/**
				 * @typedef iterator_type
				 * The type of iterator used by this stream
				 */
				using iterator_type = /*scl::stream::*/details::iterator::BaseStreamIterator<T>;

				/**
				 * @typedef it_t
				 * The type of pointer to the iterator used by this stream
				 */
				using it_t = std::shared_ptr<iterator_type>;

				/**
				 * @typedefe value_type
				 * The value type used by this stream
				 */
				using value_type = typename iterator_type::value_type;

				/**
				 * Construct a stream a raw pointer to an iterator
				 * @param iterator being a pointer to an iterator
				 */
				explicit Stream(iterator_type* iterator) : iterator{iterator}{
				}

				/**
				 * Get a reference to the underlying iterator
				 * @return the underlying iterator
				 */
				iterator_type& it() const{ return *(this->iterator); }

			protected:
				/**
				 * @var iterator
				 * The iterator used to get values
				 */
				it_t iterator;
		};
	}
}