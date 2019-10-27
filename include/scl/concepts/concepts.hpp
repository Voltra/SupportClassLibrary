#pragma once

namespace scl{
	/**
	 * @namespace scl::concepts
	 * A lightweight concept-like library for C++11
	 */
	namespace concepts{
		/**
		 * @namespace scl::concepts::details
		 * Implementation details for concepts
		 */
		namespace details{}
	}
}

#include <scl/concepts/is_concept.h>
#include <scl/concepts/require.h>

#include <scl/concepts/Same.h>
#include <scl/concepts/DerivedFrom.h>
#include <scl/concepts/ConvertibleTo.h>

#include <scl/concepts/Signed.h>
#include <scl/concepts/Unsigned.h>

#include <scl/concepts/Integral.h>
#include <scl/concepts/SignedIntegral.h>
#include <scl/concepts/UnsignedIntegral.h>

#include <scl/concepts/Destructible.h>
#include <scl/concepts/Constructible.h>
#include <scl/concepts/DefaultConstructible.h>
#include <scl/concepts/MoveConstructible.h>
#include <scl/concepts/CopyConstructible.h>

#include <scl/concepts/MoveAssignable.h>
#include <scl/concepts/CopyAssignable.h>

#include <scl/concepts/Movable.h>
#include <scl/concepts/Copyable.h>

#include <scl/concepts/NonMovable.h>
#include <scl/concepts/NonCopyable.h>

#include <scl/concepts/EqualityComparable.h>
#include <scl/concepts/EqualityComparableWith.h>
#include <scl/concepts/InequalityComparable.h>
#include <scl/concepts/InequalityComparableWith.h>
#include <scl/concepts/LessThanComparable.h>
#include <scl/concepts/LessThanComparableWith.h>

#include <scl/concepts/SemiRegular.h>
#include <scl/concepts/Regular.h>
#include <scl/concepts/Boolean.h>

#include <scl/concepts/Swappable.h>
#include <scl/concepts/SwappableWith.h>
#include <scl/concepts/Invocable.h>
#include <scl/concepts/RegularInvocable.h>
#include <scl/concepts/Predicate.h>
#include <scl/concepts/Relation.h>

//Named requirements (that are not yet concepts in C++20)
#include <scl/concepts/AssignableWith.h>
#include <scl/concepts/Iterator.h>
#include <scl/concepts/ValueSwappable.h>
#include <scl/concepts/NullablePointer.h>
#include <scl/concepts/FunctionObject.h>
#include <scl/concepts/Hash.h>
#include <scl/concepts/CharTraits.h>
#include <scl/concepts/BinaryPredicate.h>

#ifdef SCL_CPP17
#endif