#pragma once

#include <scl/stream/details/payload.h>
#include <scl/macros.h>

namespace scl{
	namespace stream{
		namespace details{
			namespace iterator{
				template <class T>
				class StreamIterator{
					static_assert(!META::is_void<T>(), "Cannot make a StreamIterator<void>");

					public:
						using payload_type = StreamIteratorPayload<T>;
						using value_type = typename payload_type::value_type;

						/**
						 * Determines whether or not this iterator can produce another value payload
						 * @return TRUE if it does, FALSE otherwise
						 */
						virtual bool hasNext() const = 0;

						/**
						 * Retrieve the next iterator payload
						 * @return the payload
						 */
						virtual payload_type next() = 0;
				};
			}
		}
	}
}