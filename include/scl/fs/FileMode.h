#pragma once
#include "../meta/type_aliases/real_const.h"

namespace scl {
    namespace fs {
	using file_mode_value_type = scl::meta::real_const_t<char*>;

	enum class FileMode {
	    // Normal modes
	    READ,
	    WRITE,
	    APPEND,
	    // Extended modes
	    READ_EX,
	    WRITE_EX,
	    APPEND_EX,
	    // Binary modes
	    READ_BIN,
	    WRITE_BIN,
	    APPEND_BIN,
	    // Extended binary modes
	    READ_BIN_EX,
	    WRITE_BIN_EX,
	    APPEND_BIN_EX,
	};

	constexpr file_mode_value_type fileModeStr(FileMode fm) {
	    // TODO: Make this a table lookup somehow

	    switch (fm) {
		case FileMode::READ:
		    return "r";

		case FileMode::WRITE:
		    return "w";

		case FileMode::APPEND:
		    return "a";

		case FileMode::READ_EX:
		    return "r+";

		case FileMode::WRITE_EX:
		    return "w+";

		case FileMode::APPEND_EX:
		    return "a+";

		case FileMode::READ_BIN:
		    return "rb";

		case FileMode::WRITE_BIN:
		    return "wb";

		case FileMode::APPEND_BIN:
		    return "ab";

		case FileMode::READ_BIN_EX:
		    return "rb+";

		case FileMode::WRITE_BIN_EX:
		    return "wb+";

		case FileMode::APPEND_BIN_EX:
		    return "ab+";

		default:
		    return "";
	    }
	}
    }  // namespace fs
}  // namespace scl