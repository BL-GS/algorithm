/*
 * @author: BL-GS 
 * @date:   2023/5/24
 */

#pragma once
#ifndef ALGORITHM_LISTENER_LISTENER_H
#define ALGORITHM_LISTENER_LISTENER_H

#include <vector>

#include <listener/listener_interface.h>
#include <listener/numa_listener.h>
#include <listener/pmem_listener.h>

namespace algorithm::listener {

	class ListenerArray {
	private:
		std::vector<AbstractListener *> listener_array_;

	public:
		ListenerArray() = default;

		ListenerArray(const ListenerArray &other) = delete;

		ListenerArray(ListenerArray &&other) = delete;

		~ListenerArray() = default;

	public:
		void add_listener(AbstractListener *listener_ptr_) {
			listener_array_.emplace_back(listener_ptr_);
		}

		void clear_listener() {
			listener_array_.clear();
		}

	public:
		void start_record() {
			for (AbstractListener *listener_ptr: listener_array_) {
				listener_ptr->start_record();
			}
		}

		void end_record() {
			for (AbstractListener *listener_ptr: listener_array_) {
				listener_ptr->end_record();
			}
		}
	};
}

#endif//ALGORITHM_LISTENER_LISTENER_H
