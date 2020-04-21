#pragma once
#include <scl/stream/details/deprecation.h>
#include <scl/stream/details/invalid_value.h>
#include <scl/utils/Either.h>
#include <scl/utils/Optional.h>
#include <functional>

namespace scl{
	namespace stream{
		namespace details{
			/**
			 * A class representing a stream iterator's payload
			 * @tparam T being the type of the value transported by the payload
			 */
			template <class T>
			class StreamIteratorPayload{
				protected:
					/**
					 * Ensure that the payload has been generated before use
					 */
					void ensureGenerated() const{
						if(!generated){
							this->alt = gen();
							generated = true;
						}
					}

				public:
					/**
					 * The type of value stored
					 */
					using value_type = T;

					/**
					 * The type that is available to the user
					 */
					using alternative = scl::utils::Optional<value_type>;

					/**
					 * The type of function that produces the alternative
					 */
					using producer = std::function<alternative(void)>;

					/**
					 * Construct a payload with its producer function
					 * @param prod being the producer function to use
					 */
					explicit StreamIteratorPayload(producer prod) : gen{prod}, alt{}{
					}

					/**
					 * Retrieve the underlying sum type
					 * @return an Either containing a value on its left or an invalid tag on its right
					 */
					const alternative& value() const{
						ensureGenerated();
						return alt;
					}

					/**
					 * Determine whether or not the underlying sum type store an invalid tag
					 * @return TRUE if it does, FALSE otherwise
					 */
					bool isInvalid() const{
						ensureGenerated();
						return !alt.hasValue();
					}

					/**
					 * Determine whether or not the underlying sum type stores a value
					 * @return TRUE if it does, FALSE otherwise
					 */
					bool isValid() const{
						return !this->isInvalid();
					}

					/**
					 * Create a payload with a value set
					 * @param value being the value of the payload
					 * @return the instantiated payload
					 */
					constexpr static StreamIteratorPayload withValue(const T& value){
						return StreamIteratorPayload{[=]{
							return alternative::ref(value);
						}};
					}

					/**
					 * Create a payload with an invalid tag set
					 * @return the instantiated payload
					 */
					constexpr static StreamIteratorPayload withoutValue(){
						return StreamIteratorPayload{[]{
							return scl::utils::none;
						}};
					}

				protected:
					/**
					 * @var gen
					 * The producer function that gives an optional result
					 */
					producer gen;

					/**
					 * @var alt
					 * The optional result (as a cache)
					 */
					mutable alternative alt;

					/**
					 * @var generated
					 * Computation cache flag
					 */
					mutable bool generated = false;
			};
		}
	}
}