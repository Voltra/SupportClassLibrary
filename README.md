# Standard Class Library
The SCL is a set of tools providing classes and utility functions that aim to ease the development of your applications.



## Organisation

Here is the namespace hierarchy:

* scl - The englobing namespace
  * [concepts](#scl-concepts) - A concept-like library based around `static_assert` (mostly implemented as defined in [cppreference](https://en.cppreference.com/w/cpp/header/concepts))
  * [tools](#scl-tools) - General purpose utility functions
    * [make](#scl-tools-make) - Create objects/pointers through factories
    * [iostream](#scl-tools-iostream) - IOstream manipulators and utils
      * [log](#scl-tools-iostream-log) - Logging utils
        * [prefixed](#scl-tools-iostream-log-prefixed) - Log w/ a prefix
        * [surrounded](#scl-tools-iostream-log-surrounded) - Log w/ a prefix & a suffix
        * [wrap](#scl-tools-iostream-log-surrounded) - An alias for [scl::tools::iostream::log::surrounded](#scl-tools-iostream-log-surrounded)
    * [meta](#scl-tools-meta) - Template Meta Programming utils (such as `void_t`, `enable_if_t` and `constexpr` functions like `is_same`)
    * [console](#scl-tools-iostream-log) - An alias for [scl::tools::iostream::log](#scl-tools-iostream-log)
    * [cli](#scl-tools-iostream-log) - An alias for [scl::console](#scl-tools-iostream-log)
  * [exceptions](#scl-exceptions) - Exception types used in the library
  * [utils](#scl-utils) - Utility classes (and rare functions), eg. `Any` or `Opiontal` or even`toString`



Here are some macros and their meaning:

* Code base maros
  * `realConst(type)` - Gives a `const` version of the given type (equivalent to `const type const`, useful for pointer)
  * `stringLiteral` - The actual string literal type (so you don't have to learn it by heart)
  * `assert_concept` - Compile time assertions of concepts outside of a block of code (must be simple concepts, not aggregates (e.g. `Copyable ` vs `Invocable`))
  * `META` - The fully qualified namespace path to the `meta` namespace (actually is `scl::tools::meta`)
* Flags
  * `SCL_CPP14` - Define this flag to use the C++14 add-ons (e.g. `is_final_t`)
  * `SCL_CPP17` - Define this flag to use the C++17 add-ons (e.g. `is_swappable_t`)
  * `SCL_CONCEPTS_REQUIRE_IS_VOID` - Define this flag to replace `constexpr bool require()` with `constexpr void require()`
  * `SCL_USE_IOSTREAM_EXTEND` - Define this flag if you wish to use the general purpose `operator<<` for `ostream` that uses `std::to_string` for conversions
  * `SCL_ENABLE_DEPRECATED_TYPEMODS` - Define this flag if you wish to define the deprecated typemods (e.g. `result_of_t`)
  * `SCL_ENABLE_DEPRECATED_TYPECHECKS` - Define this flag if you wish to define the deprecated typechecks (e.g. `is_pod_t`)

