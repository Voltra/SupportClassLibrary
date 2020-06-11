#pragma once
#include <functional>
#include <utility>
#include <scl/macros.h>
#include <scl/tools/meta/fn_type.h>

namespace scl{
	namespace utils{
#ifdef SCL_CPP17
		template <class F, class... Args>
		std::invoke_result_t<F, Args...> invoke(F&& f, Args&&... args){
			return std::invoke(
				std::forward<F>(f),
				std::forward<Args>(args)...
			);
		}

		template <class R, class Class, class... Args>
		inline R invoke(META::memfn_type<R, Class, Args...> fn, Class* const obj, Args&&... args){
			//pointer overload
			return (obj->*fn)(
				std::forward<Args>(args)...
			);
		}

		template <class R, class Class, class... Args>
		inline R invoke(META::const_memfn_type<R, Class, Args...> fn, realConst(Class*) obj, Args&&... args){
			//pointer overload
			return (obj->*fn)(
					std::forward<Args>(args)...
			);
		}

		template <class R, class Class>
		inline R invoke(META::mem_ptr_type<R, Class> fn, const Class* obj){
			return obj->*fn;
		}
#else

		/*template <class F, class R, class... Args>
		R invoke(F&& fn, Args&&... args);*/

		template <class R, class... Args>
		inline R invoke(META::fn_sig_type<R, Args...> fn, Args&&... args){
			// raw function overload
			return fn(
				std::forward<Args>(args)...
			);
		}

		template <class R, class Class, class... Args>
		inline R invoke(META::memfn_type<R, Class, Args...> fn, Class& obj, Args&&... args){
			//default object overload
			return (obj.*fn)(
				std::forward<Args>(args)...
			);
		}

		template <class R, class Class, class... Args>
		inline R invoke(META::const_memfn_type<R, Class, Args...> fn, const Class& obj, Args&&... args){
			//const object overload
			return (obj.*fn)(
				std::forward<Args>(args)...
			);
		}

		template <class R, class Class, class... Args>
		inline R invoke(META::memfn_type<R, Class, Args...> fn, Class&& obj, Args&&... args){
			//rvalue/forward overload
			return (obj.*fn)(
				std::forward<Args>(args)...
			);
		}

		template <class R, class Class, class... Args>
		inline R invoke(META::memfn_type<R, Class, Args...> fn, Class* const obj, Args&&... args){
			//pointer overload
			return (obj->*fn)(
				std::forward<Args>(args)...
			);
		}

		template <class R, class Class, class... Args>
		inline R invoke(META::const_memfn_type<R, Class, Args...> fn, realConst(Class*) obj, Args&&... args){
			//pointer overload
			return (obj->*fn)(
					std::forward<Args>(args)...
			);
		}

		template <class R, class Class>
		inline R invoke(META::mem_ptr_type<R, Class> fn, const Class& obj){
			return obj.*fn;
		}

		template <class R, class Class>
		inline R invoke(META::mem_ptr_type<R, Class> fn, const Class* obj){
			return obj->*fn;
		}

		template <class F, class... Args>
		inline auto invoke(F&& fn, Args&&... args) SCL_RETURNS(
		std::forward<F>(fn)(
				std::forward<Args>(args)...
			)
		)// generic overload
#endif
		template <class F, class... Args>
		using invoke_result_t = decltype(invoke(std::declval<F>(), std::declval<Args>()...));
	}
}