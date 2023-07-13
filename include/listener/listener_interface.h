/*
 * @author: BL-GS 
 * @date:   2023/5/24
 */

#pragma once
#ifndef ALGORITHM_LISTENER_LISTENER_INTERFACE_H
#define ALGORITHM_LISTENER_LISTENER_INTERFACE_H

namespace algorithm::listener {

	class AbstractListener {
	public:
		virtual ~AbstractListener() = default;

	public:
		/*!
		 * @brief Start recording
		 */
		virtual void start_record() = 0;

		/*!
		 * @brief End recording
		 */
		virtual void end_record() = 0;
	};

}

#endif//ALGORITHM_LISTENER_LISTENER_INTERFACE_H
