#pragma once
#include "../meta/meta.hpp"

namespace scl {
    namespace fs {
        using file_mode_value_type = scl::meta::real_const_t<char*>;

        enum class FileMode {
            READ,
            WRITE,
            APPEND,
            READ_EX,
            WRITE_EX,
            APPEND_EX,
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

                default:
                    return "a+";
            }
        }
    }  // namespace fs
}  // namespace scl