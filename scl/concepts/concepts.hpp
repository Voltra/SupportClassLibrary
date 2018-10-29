#pragma once

namespace scl{
	namespace concepts{}
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

#include <scl/concepts/EqualityComparable.h>
#include <scl/concepts/EqualityComparableWith.h>
#include <scl/concepts/InequalityComparable.h>
#include <scl/concepts/InequalityComparableWith.h>
#include <scl/concepts/LessThanComparable.h>
#include <scl/concepts/LessThanComparableWith.h>

#include <scl/concepts/SemiRegular.h>
#include <scl/concepts/Regular.h>

#include <scl/concepts/Boolean.h>

#ifdef SCL_CPP17
#include <scl/concepts/Swappable.h>
#include <scl/concepts/SwappableWith.h>
#include <scl/concepts/Invocable.h>
#include <scl/concepts/RegularInvocable.h>
#include <scl/concepts/Predicate.h>
#include <scl/concepts/Relation.h>
#endif