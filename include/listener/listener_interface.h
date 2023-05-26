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
		virtual ~AbstractListener();

	public:
		/*!
		 * @brief Start recording
		 */
		virtual void start_record();

		/*!
		 * @brief End recording
		 */
		virtual void end_record();
	};

}

#endif//ALGORITHM_LISTENER_LISTENER_INTERFACE_H
