#pragma once

#include <scl/stream/details/iterator/BaseStreamIterator.h>
#include <scl/macros.h>
#include <iterator>
#include <scl/tools/make/make.hpp>

namespace scl{
	namespace stream{
		namespace creators{
			namespace details{
				template <class It>
				using iterator_value_type = typename std::iterator_traits<It>::value_type;

				template <class It>
				class FromStlIterator : public scl::stream::details::iterator::BaseStreamIterator<iterator_value_type<It>>{
					public:
						using value_type = iterator_value_type<It>;
						using iterator_type = scl::stream::details::iterator::BaseStreamIterator<value_type>;
						using payload_type = typename iterator_type::payload_type;

					protected:
						It begin, end;

					public:
						FromStlIterator(It begin, It end) : begin{begin}, end{end} {
						}

						bool hasNext() const override{ return begin != end; }

						payload_type next() override{
							if(!this->hasNext())
								return payload_type::withoutValue();

							const value_type& value = *begin;
							++begin;
							return payload_type::withValue(value);
						}

						/*FromStlIterator<It> clone() override{}*/
				};
			}

			template <class It>
			Stream<typename details::FromStlIterator<It>::value_type> streamFrom(It begin, It end){
				using namespace scl::tools;
				return Stream<typename details::FromStlIterator<It>::value_type>{
					make::ptr<details::FromStlIterator<It>>(begin, end)
				};
			}
		}
	}
}