#pragma once

#ifdef SCL_CPP20
	#error "The scl::concepts API should not be used in C++20 and onward, prefer using actual concepts"
#elif defined(SCL_CPP17)
	#warning "The scl::concepts API will be removed in C++20"
#endif