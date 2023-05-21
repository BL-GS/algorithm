/*
 * @author: BL-GS
 * @date: 2023.05.03
 */
#pragma once
#ifndef ALGORITHM_UTIL_LOGGER_LOGGER_H
#define ALGORITHM_UTIL_LOGGER_LOGGER_H

#include <util/logger/abstract_logger.h>
#include <util/logger/console_logger.h>
#include <util/logger/file_logger.h>

namespace algorithm::util::logger {

	/*!
	* @brief Global configuration of logger
	*/

	#ifndef LOGGER_OUTPUT
		#define LOGGER_OUTPUT CONSOLE
	#endif
	#define LOGGER_TYPE_ENUM Output_Type::LOGGER_OUTPUT
	#define LOGGER_TYPE Logger<LOGGER_TYPE_ENUM, true>

	using GlobalLoggerType = LOGGER_TYPE;

	inline GlobalLoggerType &get_global_logger() {
		return GlobalLoggerType::get_instance();
	}
}

#endif