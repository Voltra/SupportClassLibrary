# Standard Class Library
The SCL is a set of tools providing classes and utility functions that aim to ease the development of your applications.



## Organisation

Here is the namespace hierarchy:

* scl - The englobing namespace
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



Here are some macros and their meaning:

* `realConst(type)` - Gives a `const` version of the given type (equivalent to `const type const`, useful for pointer)
* `SCL_CPP14` - Define this flag to use the C++14 add-ons (e.g. `is_final_t`)
* `SCL_CPP17` - Define this flag to use the C++17 add-ons (e.g. `is_swappable_t`)
* `SCL_USE_IOSTREAM_EXTEND` - Define this flag if you wish to use the general purpose `operator<<` for `ostream` that uses `std::to_string` for conversions
* `SCL_ENABLE_DEPRECATED_TYPEMODS` - Define this flag if you wish to define the deprecated typemods (e.g. `result_of_t`)
* `SCL_ENABLE_DEPRECATED_TYPECHECKS` - Define this flag if you wish to define the deprecated typechecks (e.g. `is_pod_t`)

