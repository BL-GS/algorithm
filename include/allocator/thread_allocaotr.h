/*
 * @author: BL-GS 
 * @date:   2023/5/13
 */

#pragma once
#ifndef ALGORITHM_ALLOCATOR_THREAD_ALLOCAOTR_H
#define ALGORITHM_ALLOCATOR_THREAD_ALLOCAOTR_H

#include <cstddef>
#include <cstring>
#include <algorithm>
#include <bit>

#include <util/func/calculate.h>
#include <allocator/base_allocator.h>

namespace algorithm::allocator {

	class ReserveAllocatorPool {
	public:
		static constexpr size_t CHUNK_TYPE_AMOUNT = 6;

		static constexpr size_t ALIGN_BYTE        = 4;

		static constexpr size_t ALIGN_SIZE        = (1 << ALIGN_BYTE);

		static constexpr size_t ALLOC_PAGE_SIZE   = 1024;

	private:
		struct Chunk {
			Chunk *next_chunk_;

			Chunk(): next_chunk_(nullptr) {}
		};

		struct ChunkInfo {
			Chunk *chunk_ptr_;

			ChunkInfo(): chunk_ptr_(nullptr) {}
		};

		ChunkInfo chunk_list[CHUNK_TYPE_AMOUNT];

	public:
		ReserveAllocatorPool() = default;

		ReserveAllocatorPool(const ReserveAllocatorPool &other) = delete;

		ReserveAllocatorPool(ReserveAllocatorPool &&other) {
			std::memcpy(chunk_list, other.chunk_list, sizeof(void *) * CHUNK_TYPE_AMOUNT);
			std::fill(other.chunk_list, other.chunk_list + CHUNK_TYPE_AMOUNT, ChunkInfo{});
		}

		~ReserveAllocatorPool() {
			for (const ChunkInfo &info: chunk_list) {
				for (Chunk *cur_chunk_ptr = info.chunk_ptr_; cur_chunk_ptr != nullptr; cur_chunk_ptr = cur_chunk_ptr->next_chunk_) {
					if (util::align_to_2pow(reinterpret_cast<size_t>(cur_chunk_ptr), ALLOC_PAGE_SIZE)) {
						operator delete (cur_chunk_ptr, std::align_val_t(ALLOC_PAGE_SIZE));
					}
				}
			}
		}

	public:
		void *allocate(size_t size) {
			if (size > max_size()) {
				return operator new(size, std::align_val_t(ALIGN_SIZE));
			}

			int chunk_idx = get_chunk_idx(size);
			if (chunk_list[chunk_idx].chunk_ptr_ == nullptr) { // No spare chunk left
				size_t chunk_size = get_chunk_size(chunk_idx);
				uint8_t *res_chunk_ptr = static_cast<uint8_t *>(operator new(ALLOC_PAGE_SIZE, std::align_val_t(ALLOC_PAGE_SIZE)));

				uint8_t *start_chunk_ptr = res_chunk_ptr + chunk_size;
				uint8_t *end_chunk_ptr   = res_chunk_ptr + ALLOC_PAGE_SIZE;
				uint8_t *last_chunk_ptr  = end_chunk_ptr - chunk_size;

		        chunk_list[chunk_idx].chunk_ptr_ = reinterpret_cast<Chunk *>(start_chunk_ptr);

				for (uint8_t *cur_ptr = start_chunk_ptr; cur_ptr != last_chunk_ptr; cur_ptr += chunk_size) {
					Chunk *cur_chunk_ptr = reinterpret_cast<Chunk *>(cur_ptr);
					cur_chunk_ptr->next_chunk_ = reinterpret_cast<Chunk *>(cur_ptr + chunk_size);
				}
				reinterpret_cast<Chunk *>(last_chunk_ptr)->next_chunk_ = nullptr;

				return res_chunk_ptr;
			}

			Chunk *res_chunk_ptr = chunk_list[chunk_idx].chunk_ptr_;
			chunk_list[chunk_idx].chunk_ptr_ = res_chunk_ptr->next_chunk_;
			return res_chunk_ptr;
		}

		void deallocate(void *ptr, size_t size) {
			if (size > max_size()) {
				operator delete (ptr, std::align_val_t(ALIGN_SIZE));
			}

			int chunk_idx = get_chunk_idx(size);

			Chunk *res_chunk_ptr = reinterpret_cast<Chunk *>(ptr);
			res_chunk_ptr->next_chunk_ = chunk_list[chunk_idx].chunk_ptr_;
		}

		void *reallocate(void *ptr, size_t old_size, size_t new_size) {
			if (old_size > max_size() && new_size > max_size()) {
				return std::realloc(ptr, new_size);
			}

			if (new_size > max_size()) {
				void *new_chunk_ptr = std::malloc(new_size);
				std::memcpy(new_chunk_ptr, ptr, old_size);
				deallocate(ptr, old_size);
				return new_chunk_ptr;
			}
			else if (old_size > max_size()) {
				void *new_chunk_ptr = allocate(new_size);
				std::memcpy(new_chunk_ptr, ptr, old_size);
				std::free(ptr);
				return new_chunk_ptr;
			}

			int old_chunk_idx = get_chunk_idx(old_size);
			int new_chunk_idx = get_chunk_idx(new_size);

			if (old_size == new_size) { return ptr; }

			void *new_chunk_ptr = allocate(new_size);
			std::memcpy(new_chunk_ptr, ptr, old_size);
			deallocate(ptr, old_size);
			return new_chunk_ptr;
		}

	private:
		static constexpr size_t min_size() {
			constexpr size_t MIN_SIZE = ALIGN_SIZE;
			// The min space should be larger than a pointer
			static_assert(MIN_SIZE >= sizeof(void *));

			return MIN_SIZE;
		}

		static constexpr size_t max_size() {
			constexpr size_t MAX_SIZE = ALIGN_SIZE << (CHUNK_TYPE_AMOUNT - 1);

			static_assert(MAX_SIZE >= min_size(), "The max size should be larger than min size");
			static_assert(MAX_SIZE <= ALLOC_PAGE_SIZE, "The max size should be smaller than allocated chunk");

			return MAX_SIZE;
		}

		static constexpr int get_chunk_idx(size_t size) {
			size_t offset = std::bit_width(size) - 1;

			if (offset < ALIGN_BYTE) { return 0; }

			size_t floor_size = 1 << offset;
			if (floor_size == size) {
				return static_cast<int>((offset - ALIGN_BYTE));
			}
			return static_cast<int>((offset - (ALIGN_BYTE - 1)));
		}

		static constexpr size_t get_chunk_size(size_t idx) {
			return min_size() << idx;
		}

		static constexpr size_t get_chunk_amount(size_t chunk_idx) {
			return ALLOC_PAGE_SIZE / get_chunk_size(chunk_idx);
		}
	};

	inline static thread_local ReserveAllocatorPool thread_allocator_pool;

}

#endif//ALGORITHM_ALLOCATOR_THREAD_ALLOCAOTR_H
