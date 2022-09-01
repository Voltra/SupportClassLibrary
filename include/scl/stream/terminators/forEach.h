#pragma once
#include "../../meta/type_aliases/callables.h"
#include "../../meta/type_aliases/stl.h"
#include "../Stream.h"
#include "../details/EndStreamIterator.h"

namespace scl {
    namespace stream {
	namespace terminators {
	    namespace details {
		template <class T>
		using consumer_type = std::function<void(const T&)>;

		/**
		 * For each terminator
		 * @tparam T being the value type
		 */
		template <class T, class ParentIterator>
		class ForEachTerminator final
		    : public scl::stream::details::EndStreamIterator<void, T, ParentIterator> {
		    public:
			using iterator_type
			    = scl::stream::details::EndStreamIterator<void, T, ParentIterator>;
			using value_type = typename iterator_type::value_type;
			using payload_type = typename iterator_type::payload_type;
			using result_type = typename iterator_type::result_type;
			using parent_iterator_type = typename iterator_type::parent_iterator_type;
			using parent_type = typename iterator_type::parent_type;

			/**
			 * @typedef consumer_type
			 * The type of functions to call on each element
			 */
			using consumer_type
			    = scl::stream::terminators::details::consumer_type<value_type>;

			/**
			 * Construct a terminator from its parent and a consumer
			 * @param p being the parent iterator
			 * @param c being the consumer function
			 */
			ForEachTerminator(parent_iterator_type p, consumer_type c)
			    : iterator_type(std::move(p)), consumer{std::move(c)} {}

			void process() final {
			    for (auto payload : *this) {
				if (payload.isValid()) {
				    auto&& opt = payload.value();
				    opt.ifSome(consumer);
				}
			    }
			};

		    protected:
			/**
			 * @var consumer
			 * the function to call on each element
			 */
			consumer_type consumer;
		};

		/**
		 * forEach toolbox tag
		 * @tparam T being the value type
		 */
		template <class T>
		struct for_each_terminator_payload {
			/**
			 * @typedef callback_t
			 * The function type of a callback
			 */
			using callback_t = consumer_type<T>;

			/**
			 * @var callback
			 * the callback
			 */
			callback_t callback;
		};
	    }  // namespace details

	    /**
	     * Invoke a function on each element of a stream
	     * @tparam F being the callback type (deduced)
	     * @tparam Fn being the function wrapper type (computed)
	     * @tparam T being the value type (computed)
	     * @param callback being the callback
	     * @return a toolbox tag for pipe operator
	     */
	    template <class F, class T = scl::meta::remove_cv_ref_t<scl::meta::first_arg_t<F>>>
	    details::for_each_terminator_payload<T> forEach(F&& callback) {
		return {std::forward<F>(callback)};
	    }

	    /**
	     * Invoke a function on each element of a stream by providing the type arguments
	     * @tparam T being the value type
	     * @param cb being the callback function
	     * @return a toolbox tag for pipe operator
	     */
	    template <class T>
	    details::for_each_terminator_payload<T> forEach_(
		typename details::for_each_terminator_payload<T>::callback_t&& cb) {
		return {std::move(cb)};
	    }

	    /**
	     * Pipe operator overload for map toolbox tags
	     * @tparam T being the type to map from
	     * @tparam U being the type to map to
	     * @return The mapped stream
	     */
	    template <class T, class It>
	    void operator|(Stream<T, It>&& lhs, details::for_each_terminator_payload<T>&& rhs) {
		details::ForEachTerminator<T, It> forEachTerminator{std::move(lhs.it()),
								    std::move(rhs.callback)};

		forEachTerminator.process();
	    }
	}  // namespace terminators
    }      // namespace stream
}  // namespace scl