#include <cstdio>
#include <string>

#include "../macros.h"
#include "../meta/meta.hpp"
#include "../utils/utils.hpp"
#include "./FileMode.h"
#include "./SeekMode.h"

namespace scl {
    namespace fs {
        class RawFile {
        public:
            using file_type = std::FILE*;
            using size_type = std::size_t;

            friend scl::utils::Optional<RawFile>;

            static inline bool remove(const char* fileName) noexcept {
                return std::remove(fileName) == 0;
            }

            static inline bool rename(const char* oldFileName, const char* newFileName) noexcept {
                return std::rename(oldFileName, newFileName) == 0;
            }

        protected:
            file_type fd;
            bool closable = true;

            void close() noexcept {
                if(closable) {
                    std::fclose(fd);
                }
            }

            RawFile(file_type fd, bool closable) : fd{fd}, closable{closable} {}

        public:
            static SCL_CONSTEXPR17 RawFile cstdin() noexcept { return RawFile{stdin, false}; }

            static SCL_CONSTEXPR17 RawFile cstdout() noexcept { return RawFile{stdout, false}; }

            static SCL_CONSTEXPR17 RawFile cstderr() noexcept { return RawFile{stderr, false}; }

            static scl::utils::Optional<RawFile> tmp() noexcept {
                auto* fd = std::tmpfile();

                if (fd == nullptr) {
                    return {};
                }

                return scl::utils::Optional<RawFile>::inplace(fd, true);
            }

            RawFile(scl::meta::real_const_t<char*> path, FileMode mode)
                : RawFile(std::fopen(path, fileModeStr(mode)), true) {}

            RawFile(const RawFile&) = delete;
            RawFile& operator=(const RawFile&) = delete;

            RawFile(RawFile&& other) noexcept : fd{scl::utils::exchange(other.fd, nullptr)}, closable{scl::utils::exchange(other.closable, false)} {}

            RawFile& operator=(RawFile&& rhs) noexcept {
                close();
                fd = scl::utils::exchange(rhs.fd, nullptr);
                closable = scl::utils::exchange(rhs.closable, false);
                return *this;
            }

            /**
             * Writes any unwritten data from the stream's buffer to the associated output device
             * @note Undefined Behavior on input streams
             * @see https://en.cppreference.com/w/cpp/io/c/fflush
             * @return TRUE if the flush was successful, FALSE otherwise
             */
            bool flush() & noexcept { return std::fflush(fd) == 0; }

            std::string read(size_type count) & {
                std::string ret{};
                ret.resize(count, '\0');
                std::fread(&ret[0], sizeof(char), count, fd);
                ret.shrink_to_fit();
                return ret;
            }

            std::size_t read(char* buffer, size_type bufferMaxSize) & noexcept {
                return std::fread(buffer, sizeof(char), bufferMaxSize, fd);
            }

            std::size_t write(scl::utils::StringView str) & noexcept {
                return std::fwrite(str.data(), sizeof(char), str.size(), fd);
            }

            scl::utils::Optional<int> getc() & noexcept {
                int ret = std::fgetc(fd);
                return ret == EOF ? scl::utils::none : scl::utils::Optional<int>{ret};
            }

            std::string gets(size_type count) & {
                std::string ret{};
                ret.resize(count, '\0');
                std::fgets(&ret[0], count, fd);
                ret.shrink_to_fit();
                return ret;
            }

            bool putc(int ch) & noexcept { return std::fputc(ch, fd) != EOF; }

            bool puts(const char* str) & noexcept {  // TODO: Make a string view version
                return std::fputs(str, fd) != EOF;
            }

            bool ungetc(int ch) & noexcept { return std::ungetc(ch, fd) != EOF; }

            scl::utils::Optional<std::wint_t> getwc() & noexcept {
                auto ret = std::fgetwc(fd);
                return ret == WEOF ? scl::utils::none : scl::utils::Optional<std::wint_t>{ret};
            }

            std::wstring getws(size_type count) & {
                std::wstring ret{};
                ret.resize(count, '\0');
                std::fgetws(&ret[0], count, fd);
                ret.shrink_to_fit();
                return ret;
            }

            bool putwc(std::wint_t ch) & noexcept { return std::fputwc(ch, fd) != WEOF; }

            bool putws(const wchar_t* str) & noexcept {  // TODO: Make a string view version
                return std::fputws(str, fd) != WEOF;
            }

            bool ungetwc(std::wint_t ch) & noexcept { return std::ungetwc(ch, fd) != WEOF; }

            long tell() const& noexcept { return std::ftell(fd); }

            scl::utils::Optional<std::fpos_t> getpos() const& noexcept {
                std::fpos_t pos;
                auto ret = std::fgetpos(fd, &pos);
                return ret == 0 ? scl::utils::Optional<std::fpos_t>{std::move(pos)}
                                : scl::utils::none;
            }

            bool seek(long offset, SeekMode origin) {
                return std::fseek(fd, offset, seekModeCode(origin)) == 0;
            }

            bool setpos(const std::fpos_t& pos) & noexcept { return std::fsetpos(fd, &pos) == 0; }

            void rewind() & noexcept { std::rewind(fd); }

            void clearerr() & noexcept { std::clearerr(fd); }

            bool eof() const& noexcept {
                return std::feof(fd);
            }

            bool error() const& noexcept {
                return std::ferror(fd);
            }

            ~RawFile() {
                close();
            }
        };
    }  // namespace fs
}  // namespace scl
