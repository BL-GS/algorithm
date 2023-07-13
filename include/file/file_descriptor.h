/*
 * @author: BL-GS 
 * @date:   2023/7/6
 */

#pragma once
#ifndef ALGORITHM_FILE_FILE_DESCRIPTOR_H
#define ALGORITHM_FILE_FILE_DESCRIPTOR_H

#include <string_view>
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <span>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <logger/logger.h>
#include <util/enum_operator.h>

namespace algorithm::file {

	enum class FileOpenType : int {
		ReadOnly = O_RDONLY,
		WriteOnly = O_WRONLY,
		ReadWrite = O_RDWR,
		Create = O_CREAT,
		Truncate = O_TRUNC,
		Append = O_APPEND
	};

	enum class FileMmapProt : int {
		Executable = PROT_EXEC,
		Readable = PROT_READ,
		Writable = PROT_WRITE,
		None = PROT_NONE
	};

	enum class FileMmapFlag : int {
		Private = MAP_PRIVATE,
		Shared = MAP_SHARED,
		Anonymous = MAP_ANONYMOUS,
		DenyWrite = MAP_DENYWRITE,
		Locked = MAP_LOCKED
	};

	enum class FileRelocateType: int {
		StartLoc,
		CurrentLoc,
		EndLoc
	};
}

template<> struct algorithm::util::EnableEnumOperator<algorithm::file::FileOpenType> {
	static constexpr bool enable = true;
};

template<> struct algorithm::util::EnableEnumOperator<algorithm::file::FileMmapProt> {
	static constexpr bool enable = true;
};

template<> struct algorithm::util::EnableEnumOperator<algorithm::file::FileMmapFlag> {
	static constexpr bool enable = true;
};

template<> struct algorithm::util::EnableEnumOperator<algorithm::file::FileRelocateType> {
	static constexpr bool enable = true;
};

namespace algorithm::file {

	class FileMapper {
	private:
		int fd_;

		std::span<uint8_t> map_area_;

	public:
		FileMapper(): fd_(-1) {}

		FileMapper(int fd, size_t size, FileMmapProt prot, FileMmapFlag flag) {
			void *start_ptr = mmap(nullptr, size, static_cast<int>(prot), static_cast<int>(flag), fd, 0);
			if (start_ptr == nullptr) {
				util::logger::logger_error("Fail to mmap file: ", fd);
				util::logger::logger_error(std::strerror(errno));
			}
			else {
				fd_ = fd;
				map_area_ = std::span<uint8_t> (
				        static_cast<uint8_t *>(start_ptr),
				        size
				);
			}
		}

		FileMapper(FileMapper &&other) noexcept : fd_(other.fd_), map_area_(other.map_area_) {
			other.fd_ = -1;
		}

		~FileMapper() {
			if (fd_ != -1) {
				munmap(map_area_.data(), map_area_.size());
			}
		}

	public:
		std::span<uint8_t> get_map_area() const {
			return map_area_;
		}
	};

	class FileDescriptor {
	private:
		int fd_;

		struct stat file_stat_;

	public:
		FileDescriptor(): fd_(-1) {}

		FileDescriptor(std::string_view file_path, FileOpenType type):
                fd_(open(file_path.data(), static_cast<int>(type))) {
			if (fd_ == -1) {
				util::logger::logger_error("Fail to open file: ", file_path);
				util::logger::logger_error(std::strerror(errno));
			}

			stat(file_path.data(), &file_stat_);
		}

		~FileDescriptor() {
			if (fd_ != -1) {
				close(fd_);
			}
		}

	public:
		FileMapper get_mapper(FileMmapProt prot, FileMmapFlag flag) const {
			return FileMapper(fd_, file_stat_.st_size, prot, flag);
		}

		FileMapper get_mapper(size_t size, FileMmapProt prot, FileMmapFlag flag) const {
			return FileMapper(fd_, size, prot, flag);
		}

	public:
		ssize_t read_forward(size_t size, void *output_ptr) const {
			return read(fd_, output_ptr, size);
		}

		ssize_t write_forward(size_t size, void *input_ptr) const {
			return write(fd_, input_ptr, size);
		}

		off_t relocate(int offset, FileRelocateType type) const {
			return lseek(fd_, offset, static_cast<int>(type));
		}
	};

}

#endif//ALGORITHM_FILE_FILE_DESCRIPTOR_H
