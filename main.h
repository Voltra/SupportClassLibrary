#pragma once

#include <scl/utils/toString.h>
#include <scl/macros.h>
#include <scl/concepts/require.h>
#include <sstream>
#include <string>
#include <memory>


namespace std{
	template <class T>
	inline string to_string(T* ptr){ stringstream ss; ss << ptr; return ss.str(); }

	template <class T>
	inline string to_string(unique_ptr<T>& uptr){ return to_string(uptr.get()); }

	template <class T>
	ostream& operator<<(ostream& os, unique_ptr<T>& uptr){
		return os << to_string(uptr);
	}
}


/*namespace scl{
	namespace utils{
		template <class T>
		struct ToString<T*>{
			std::string operator()(realConst(T*) ptr) const{
				std::stringstream ss;
				ss << ptr;
				return ss.str();
			}
		};

		template <>
		struct ToString<int>{
			std::string operator()(const int& i) const{
				return std::to_string(i);
			}
		};
	}
}*/
