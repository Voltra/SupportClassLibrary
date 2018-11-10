#pragma once

namespace scl{
	namespace tools{
		/**
		 * @namespace scl::tools::meta
		 * A namespae englobing all the TMP tools
		 */
		namespace meta{}
	}
}

#include <scl/tools/meta/void_t.h>
#include <scl/tools/meta/exists.h>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/conditional.h>
#include <scl/tools/meta/is_convertible.h>
#include <scl/tools/meta/is_same.h>
#include <scl/tools/meta/fn_type.h>
#include <scl/tools/meta/fn_cast.h>
#include <scl/tools/meta/can_call.h>
#include <scl/tools/meta/type_check.h>
#include <scl/tools/meta/swap.h>
#include <scl/tools/meta/type_mod.h>
#include <scl/tools/meta/type_query.h>
#include <scl/tools/meta/defines_scl_to_string.h>
#include <scl/tools/meta/defines_std_to_string.h>
#include <scl/tools/meta/defines_ostream_operator.h>
#include <scl/tools/meta/constexpr_assert.h>