#pragma once

#ifdef SCL_CPP20
	#error "The scl::stream API should not be used in C++20 and onward, prefer std::ranges"
#elif defined(SCL_CPP17)
	#warning "The scl::stream API will be removed in C++20"
#endif