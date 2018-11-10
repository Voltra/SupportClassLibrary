#pragma once

#include "../macros.h"
#include "require.h"
#include "Invocable.h"
#include "../tools/meta/type_check.h"

namespace scl{
	namespace concepts{
		/**
		 * FunctionObject concept, a type F is a function object that accepts Args... as arguments
		 * if it is an object and is invocable with those arguments
		 * @tparam F being the type to check against
		 * @tparam Args being the desired arguments' types
		 */
		template <class F, class... Args>
		struct FunctionObject{
			constexpr operator bool() const{
				return META::is_object<F>()
				&& Invocable<F, Args...>{};
			}
		};
	}
}