#include <cstdio>
#include <string>

#include "../macros.h"
#include "../utils/Optional.h"
#include "../utils/StringView.h"
#include "./FileMode.h"
#include "./SeekMode.h"

namespace scl {
    namespace fs {
	/**
	 * Remove a file
	 * @param fileName being the name of the file to remove
	 * @return TRUE if the file was removed, FALSE otherwise
	 */
	inline static bool remove(const char* fileName) noexcept {
	    return std::remove(fileName) == 0;
	}
	/**
	 * Remove a file
	 * @param oldFileName being the name of the file before renaming
	 * @param newFileName being the name of the file after renaming
	 * @return TRUE if the file was renamed, FALSE otherwise
	 */
	inline static bool rename(const char* oldFileName, const char* newFileName) noexcept {
	    return std::rename(oldFileName, newFileName) == 0;
	}

	/**
	 * RAII wrapper around std::FILE*
	 */
	class RawFile {
	    public:
		using file_type = std::FILE*;
		using size_type = std::size_t;

		friend scl::utils::Optional<RawFile>;
		friend scl::utils::RawStorage<RawFile>;
		friend scl::utils::details::RawStorageImpl<RawFile,
							   scl::utils::RawStorage<RawFile>>;

	    protected:
		file_type fd;
		bool closable = true;

		void close() noexcept {
		    if (closable) {
			std::fclose(fd);
		    }
		}

		RawFile(file_type fd, bool closable) : fd{fd}, closable{closable} {}

	    public:
		/**
		 * @return A wrapper around the standard input
		 * @note Auto-closing the file is disabled, so you can make multiple instances via
		 * this method if needed
		 */
		static SCL_CONSTEXPR17 RawFile cstdin() noexcept { return RawFile{stdin, false}; }

		/**
		 * @return A wrapper around the standard output
		 * @note Auto-closing the file is disabled, so you can make multiple instances via
		 * this method if needed
		 */
		static SCL_CONSTEXPR17 RawFile cstdout() noexcept { return RawFile{stdout, false}; }

		/**
		 * @return A wrapper around the standard error output
		 * @note Auto-closing the file is disabled, so you can make multiple instances via
		 * this method if needed
		 */
		static SCL_CONSTEXPR17 RawFile cstderr() noexcept { return RawFile{stderr, false}; }

		template <class Engine = scl::utils::details::DefaultOptionalEngine<RawFile>>
		static scl::utils::Optional<RawFile, Engine> tmp() noexcept {
		    auto* fd = std::tmpfile();

		    if (fd == nullptr) {
			return {};
		    }

		    return RawFile{fd, true};
		}

		RawFile(scl::meta::real_const_t<char*> path, FileMode mode)
		    : RawFile(std::fopen(path, fileModeStr(mode)), true) {}

		RawFile(const RawFile&) = delete;
		RawFile& operator=(const RawFile&) = delete;

		RawFile(RawFile&& other) noexcept
		    : fd{scl::utils::exchange(other.fd, nullptr)},
		      closable{scl::utils::exchange(other.closable, false)} {}

		RawFile& operator=(RawFile&& rhs) noexcept {
		    close();
		    fd = scl::utils::exchange(rhs.fd, nullptr);
		    closable = scl::utils::exchange(rhs.closable, false);
		    return *this;
		}

		/**
		 * Writes any unwritten data from the stream's buffer to the associated output
		 * device
		 * @note Undefined Behavior on input streams
		 * @see https://en.cppreference.com/w/cpp/io/c/fflush
		 * @return TRUE if the flush was successful, FALSE otherwise
		 */
		bool flush() & noexcept { return std::fflush(fd) == 0; }

		/**
		 * Reads up to @p count characters
		 * @param count
		 * @return the content that was read
		 * @post return.size() \<= @p count
		 */
		std::string read(size_type count) & {
		    std::string ret{};
		    ret.resize(count, '\0');
		    std::fread(&ret[0], sizeof(char), count, fd);
		    ret.shrink_to_fit();
		    return ret;
		}

		/**
		 * Read characters into the given buffer
		 * @pre @p buffer can store at least @p maxCount characters
		 * @param buffer being the buffer to read characters into
		 * @param maxCount being the maximum amount of characters to read (usually the
		 * amount allocated for @p buffer)
		 * @return the number of characters read
		 * @post return \<= @p maxCount
		 */
		std::size_t read(char* buffer, size_type maxCount) & noexcept {
		    return std::fread(buffer, sizeof(char), maxCount, fd);
		}

		/**
		 * Read characters into the given container
		 * @pre @p container is initialized and has memory reserved
		 * @tparam Container
		 * @param container being the container to read characters into
		 * @return the number of characters read
		 * @post return \<= @p container.capacity()
		 */
		template <class Container>
		std::size_t read(Container& container) & noexcept {
		    return std::fread(container.begin(), sizeof(char), container.capacity(), fd);
		}

		std::string readAll(size_type batchSize = 4096) & {
		    std::string ret{};
		    auto size = batchSize;

		    do {
			ret.resize(size, '\0');
			auto* ptr = &ret[size - batchSize];  // size - batchSize == index;
							     // (size=batchSize) - batchSize = 0
			std::fread(ptr, sizeof(char), batchSize, fd);
			size += batchSize;
		    } while (ok());

		    ret.shrink_to_fit();

		    return ret;
		}

		/**
		 * Write the content of a string view into the file
		 * @param str being the sequence of characters to write
		 * @return the amount of characters written to the file
		 */
		std::size_t write(scl::utils::StringView str) & noexcept {
		    return std::fwrite(str.data(), sizeof(char), str.size(), fd);
		}

		/**
		 * Read a single character
		 * @return an empty optional on error or EOF, the character read otherwise
		 */
		template <class Engine = scl::utils::details::DefaultOptionalEngine<int>>
		scl::utils::Optional<int, Engine> getc() & noexcept {
		    auto ret = std::fgetc(fd);
		    return ret == EOF ? scl::utils::none
				      : scl::utils::Optional<int, Engine>::inplace(ret);
		}

		/**
		 * Read a string of max @p count characters from the file
		 * @param count being the maximum amount of characters to read
		 * @return the string that was read
		 * @post return.size() \<= @p count
		 */
		std::string gets(int count) & {
		    std::string ret{};
		    ret.resize(count, '\0');
		    std::fgets(&ret[0], count, fd);
		    ret.shrink_to_fit();
		    return ret;
		}

		/**
		 * Write a single character
		 * @param ch being the character to write
		 * @return TRUE if @p ch was successfully written, FALSE otherwise
		 */
		bool putc(int ch) & noexcept { return std::fputc(ch, fd) != EOF; }

		/**
		 * Write a NULL TERMINATED string
		 * @pre @p str is null-terminated
		 * @param str being the string to write
		 * @return TRUE if @p str was successfully written, FALSE otherwise
		 */
		bool puts(const char* str) & noexcept { return std::fputs(str, fd) != EOF; }

		/**
		 * Pushes @p ch into the input stream associated with the file (so that next
		 * character reads will pick it up)
		 * @param ch being the character to write
		 * @return TRUE if @p ch was successfully written, FALSE otherwise
		 */
		bool ungetc(int ch) & noexcept { return std::ungetc(ch, fd) != EOF; }

		/**
		 * Read a single, wide character
		 * @return an empty optional on error or EOF, the character read otherwise
		 */
		template <class Engine = scl::utils::details::DefaultOptionalEngine<std::wint_t>>
		scl::utils::Optional<std::wint_t, Engine> getwc() & noexcept {
		    auto ret = std::fgetwc(fd);
		    return ret == WEOF ? scl::utils::none
				       : scl::utils::Optional<std::wint_t, Engine>::inplace(ret);
		}

		/**
		 * Read a wide string of max @p count characters from the file
		 * @param count being the maximum amount of wide characters to read
		 * @return the string that was read
		 * @post return.size() \<= @p count
		 */
		std::wstring getws(int count) & {
		    std::wstring ret{};
		    ret.resize(count, '\0');
		    std::fgetws(&ret[0], count, fd);
		    ret.shrink_to_fit();
		    return ret;
		}

		/**
		 * Write a single wide character
		 * @param ch being the character to write
		 * @return TRUE if @p ch was successfully written, FALSE otherwise
		 */
		bool putwc(std::wint_t ch) & noexcept { return std::fputwc(ch, fd) != WEOF; }

		/**
		 * Write a NULL TERMINATED wide string
		 * @pre @p str is null-terminated
		 * @param str being the string to write
		 * @return TRUE if @p str was successfully written, FALSE otherwise
		 */
		bool putws(const wchar_t* str) & noexcept { return std::fputws(str, fd) != WEOF; }

		/**
		 * Pushes @p ch into the input stream associated with the file (so that next
		 * character reads will pick it up)
		 * @param ch being the wide character to write
		 * @return TRUE if @p ch was successfully written, FALSE otherwise
		 */
		bool ungetwc(std::wint_t ch) & noexcept { return std::ungetwc(ch, fd) != WEOF; }

		/**
		 * Get the current file position indicator
		 * @return the position within the file (in bytes if open in binary mode, otherwise
		 * its meaning is unspecified and can only be used as input to
		 * scl::fs::RawFile::seek)
		 * @post return >= 0
		 */
		long tell() const& noexcept { return std::ftell(fd); }

		/**
		 * Get the current file position indicator and multi-byte parsing state, if any
		 * @return the position state (only meaningful as input of scl::fs::RawFile::setpos)
		 */
		template <class Engine = scl::utils::details::DefaultOptionalEngine<std::fpos_t>>
		scl::utils::Optional<std::fpos_t, Engine> getpos() const& noexcept {
		    std::fpos_t pos;
		    auto ret = std::fgetpos(fd, &pos);
		    return ret == 0 ? scl::utils::Optional<std::fpos_t, Engine>{std::move(pos)}
				    : scl::utils::none;
		}

		/**
		 * Set the current file position indicator based on an @p origin and an @p offset
		 * @param offset being the offset to move the indicator by
		 * @param origin being the origin of the movement (begin, end, current pos, set)
		 * @return
		 */
		bool seek(long offset, SeekMode origin) {
		    return std::fseek(fd, offset, seekModeCode(origin)) == 0;
		}

		/**
		 * Set the current file position indicator and multi-byte parsing state, if any
		 * @param pos being the position state obtained via scl::fs::RawFile::getpos
		 * @return TRUE if successful, FALSE otherwise
		 */
		bool setpos(const std::fpos_t& pos) & noexcept {
		    return std::fsetpos(fd, &pos) == 0;
		}

		/**
		 * Set the current file position indicator and multi-byte parsing state, if any
		 * @param pos being the position state obtained via scl::fs::RawFile::getpos
		 * @return TRUE if successful, FALSE otherwise
		 */
		template <class Engine = scl::utils::details::DefaultOptionalEngine<std::fpos_t>,
			  class EngineRet = scl::utils::details::DefaultOptionalEngine<bool>>
		scl::utils::Optional<bool, EngineRet> setposOpt(
		    const scl::utils::Optional<std::fpos_t>& opt) & noexcept {
		    return opt.map(
			[=](const std::fpos_t& pos) -> bool { return this->setpos(pos); });
		}

		/**
		 * Move the current file position indicator to the beginning of the file
		 */
		void rewind() & noexcept { std::rewind(fd); }

		/**
		 * Reset the error and EOF flags
		 */
		void clearerr() & noexcept { std::clearerr(fd); }

		/**
		 * Whether the file reached EOF
		 */
		bool eof() const& noexcept { return std::feof(fd); }

		/**
		 * Whether there was an error in the last operation
		 */
		bool error() const& noexcept { return std::ferror(fd); }

		bool ok() const& noexcept { return !(eof() || error()); }

		~RawFile() noexcept { close(); }
	};
    }  // namespace fs
}  // namespace scl
