#pragma once

#include "is_concept.h"

namespace scl{
	namespace concepts{
#ifdef SCL_CONCEPTS_REQUIRE_IS_VOID
		/**
		 * Require a concept
		 * @tparam ConceptType being the concept type to require (eg. Integral<T>)
		 * @param c being an instance of said ConceptType
		 * @warning Will not compile if the concept's requirements are not met
		 */
		template <class ConceptType>
		constexpr void require(ConceptType c){
			static_assert(is_concept<ConceptType>(), "Cannot require a non Concept type");
			static_assert(c, "");
		}
#else
		/**
		 * Require a concept
		 * @tparam ConceptType being the concept type to require (eg. Integral<T>)
		 * @param c being an instance of said ConceptType
		 * @return true
		 * @warning Will not compile if the concept's requirements are not met
		 */
		template <class ConceptType>
		constexpr bool require(ConceptType c){
			static_assert(is_concept<ConceptType>(), "Cannot require a non Concept type");
			static_assert(c, "");
			/*
			 Concepts are designed as (usually) default constructible
			 with a defined operator bool() that asserts the different requirements
			 for the concept
			 */
			return true;
		}
#endif
	}
}