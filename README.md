# Support Class Library
***<u>WARNING:</u> This is currently a Work In Progress***



The SCL is a set of tools providing classes and utility functions that aim to ease the development of your applications.



You can have a look at the documentation [here](https://voltra.github.io/SupportClassLibrary/doc/html/).



## Organisation

Here is the namespace hierarchy:

* [scl](./include/scl/scl.hpp) - The englobing namespace
  * [concepts](./include/scl/concepts/concepts.hpp) - A concept-like library based around `static_assert` (mostly implemented as defined in [cppreference](https://en.cppreference.com/w/cpp/header/concepts))
  * [tools](./include/scl/tools/tools.hpp) - General purpose utility functions
    * [make](./include/scl/tools/make/make.hpp) - Create objects/pointers through factories
    * [iostream](./include/scl/tools/iostream/iostream.hpp) - IOstream manipulators and utils
      * [log](./include/scl/tool/iostream/log/log.hpp) - Logging utils
        * [prefixed](./include/scl/tool/iostream/log/prefixed.h) - Log w/ a prefix
        * [surrounded](./include/scl/tool/iostream/log/surrounded.h) - Log w/ a prefix & a suffix
        * [wrap](./include/scl/tool/iostream/log/log.hpp) - An alias for [scl::tools::iostream::log::surrounded](./include/scl/tool/iostream/log/surrounded.h)
    * [meta](#scl-tools-meta) - Template Meta Programming utils (such as `void_t`, `enable_if_t` and `constexpr` functions like `is_same` or even handy stuff like `constexpr_assert`)
    * [console](./include/scl/tool/iostream/log/log.hpp) - An alias for [scl::tools::iostream::log](./include/scl/tool/iostream/log/log.hpp)
    * [cli](./include/scl/tool/iostream/log/log.hpp) - An alias for [scl::console](./include/scl/tool/iostream/log/log.hpp)
  * [exceptions](./include/scl/exceptions/exceptions.hpp) - Exception types used in the library
  * [utils](./include/scl/utils/utils.hpp) - Utility classes (and rare functions), eg. `Any` or `Opiontal` or even`toString`
  * [stream](./include/scl/stream/stream.hpp) - The stream API exposing stream creators, operations and terminators
    * [details](./include/scl/stream/details/details.hpp) - Implementation details for streams (including the base iterator types)
    * [creators](./include/scl/stream/creators/creators.hpp) - Grouping stream creation functions
    * [operators](./include/scl/stream/operators/operators.hpp) - Grouping intermediate stream operation functions
    * [terminators](./include/scl/stream/terminators/terminators.hpp) - Grouping end of stream operation functions 
  * [async](./async/async.hpp) - The asynchronous programming API
  * [http]( ./http/http.hpp ) - The HTTP API



Here are some [macros](./include/scl/macros.h) and their meaning:

* Code based macros
  * `realConst(type)` - Gives a `const` version of the given type (equivalent to `const type const`, useful for pointers)
  * `stringLiteral` - The actual string literal type (so you don't have to learn it by heart)
  * `assert_concept` - Compile time assertions of concepts outside of a block of code (must be simple concepts, not aggregates (e.g. `Copyable ` vs `Invocable`))
  * `static_require` - Require concepts without giving an additional error message
  * `META` - The fully qualified namespace path to the `meta` namespace (actually is `scl::tools::meta`)
  * `asString` - The function that converts a variable to `std::string` using its `ToString` specialization
* Flags
  * `SCL_CPP14` - Define this flag to use the C++14 add-ons (e.g. `is_final_t`)
  * `SCL_CPP17` - Define this flag to use the C++17 add-ons (e.g. `is_swappable_t`)
  * `SCL_CONCEPTS_REQUIRE_IS_VOID` - Define this flag to replace `constexpr bool require()` with `constexpr void require()`
  * `SCL_USE_IOSTREAM_EXTEND` - Define this flag if you wish to use the general purpose `operator<<` for `ostream` that uses `std::to_string` and `scl::utils::toString` for conversions
  * `SCL_ENABLE_DEPRECATED_TYPEMODS` - Define this flag if you wish to define the deprecated typemods (e.g. `result_of_t`)
  * `SCL_ENABLE_DEPRECATED_TYPECHECKS` - Define this flag if you wish to define the deprecated typechecks (e.g. `is_pod_t`)

