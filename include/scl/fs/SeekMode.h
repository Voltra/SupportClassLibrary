#pragma once
#include <cstdio>

namespace scl {
    namespace fs {
	enum class SeekMode {
	    SET,
	    CURRENT,
	    END
	};

	inline constexpr int seekModeCode(SeekMode seekMode) {
	    switch (seekMode) {
		case SeekMode::SET:
		    return SEEK_SET;

		case SeekMode::CURRENT:
		    return SEEK_CUR;

		default:
		    return SEEK_END;
	    }
	}
    }  // namespace fs
}  // namespace scl
