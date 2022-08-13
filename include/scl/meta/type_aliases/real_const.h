#pragma once

namespace scl {
    namespace meta {
        namespace details {
            /**
             * Get the appropriate const version of a type
             * @tparam T the type which will be const qualified
             */
            template <class T>
            struct real_const {
                using type = const decay_t<remove_cv_ref_t<T>>;
            };

            /**
             * Specialization for pointers
             * @tparam T being the value type
             */
            template <class T>
            struct real_const<T*> {
                using type = const T* const;
            };
        }  // namespace details

        /**
         * Get the appropriate const version of a type
         * @tparam T the type which will be const qualified
         */
        template <class T>
        using real_const_t = typename details::real_const<T>::type;
    }  // namespace meta
}  // namespace scl