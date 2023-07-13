/*
 * @author: BL-GS 
 * @date:   2023/5/26
 */

#pragma once
#ifndef ALGORITHM_UTIL_ENUM_OPERATOR_H
#define ALGORITHM_UTIL_ENUM_OPERATOR_H

#include <type_traits>

/*!
 * @brief Operator definition of enum operator
 * @class EnableEnumOperator Define constexpr param 'enable' as true to enable bit operator on enumeration type
 * @class EnableEnumPartialOperator Define constexpr param 'enable' as true to enable partial bit operator on enumeration type
 * @class EnableEnumCompareOperator Define constexpr param 'enable' as true to enable comparison operator on enumeration type
 * @class EnableEnumPartialCompareOperator Define constexpr param 'enable' as true to enable partial comparison operator on enumeration type
 */


namespace algorithm::util {

	/*!
	 * @brief To enable operator between enums, just partially specialize this template with enable = true.
	 * @tparam T The type of enumeration
	 */
	template<typename T>
		requires std::is_enum_v<T>
	struct EnableEnumOperator {
		static constexpr bool enable = false;
	};

}

template<typename EnumType>
    requires (algorithm::util::EnableEnumOperator<EnumType>::enable)
inline constexpr EnumType operator | (EnumType lhs, EnumType rhs) {
	return static_cast<EnumType>(
	        static_cast<std::underlying_type_t<EnumType>>(lhs) | static_cast<std::underlying_type_t<EnumType>>(rhs)
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumOperator<EnumType>::enable)
inline constexpr EnumType operator & (EnumType lhs, EnumType rhs) {
	return static_cast<EnumType>(
	        static_cast<std::underlying_type_t<EnumType>>(lhs) & static_cast<std::underlying_type_t<EnumType>>(rhs)
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumOperator<EnumType>::enable)
inline constexpr EnumType operator ^ (EnumType lhs, EnumType rhs) {
	return static_cast<EnumType>(
	        static_cast<std::underlying_type_t<EnumType>>(lhs) ^ static_cast<std::underlying_type_t<EnumType>>(rhs)
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumOperator<EnumType>::enable)
inline constexpr EnumType operator ~ (EnumType obj) {
	return static_cast<EnumType>(
	        static_cast<std::underlying_type_t<EnumType>>(obj)
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumOperator<EnumType>::enable)
inline constexpr EnumType &operator |= (EnumType lhs, EnumType rhs) {
	return lhs = (lhs | rhs);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumOperator<EnumType>::enable)
inline constexpr EnumType &operator &= (EnumType lhs, EnumType rhs) {
	return lhs = (lhs & rhs);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumOperator<EnumType>::enable)
inline constexpr EnumType &operator ^= (EnumType lhs, EnumType rhs) {
	return lhs = (lhs ^ rhs);
}



namespace algorithm::util {

	/*!
	 * @brief To enable partial operator between enums, just partially specialize this template with enable = true.
	 * @tparam T The type of enumeration
	 */
	template<typename T>
	    requires std::is_enum_v<T>
	struct EnableEnumPartialOperator {
		static constexpr bool enable = false;
	};

}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> operator | (std::underlying_type_t<EnumType> lhs, EnumType rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(
	        lhs | static_cast<std::underlying_type_t<EnumType>>(rhs)
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> operator & (std::underlying_type_t<EnumType> lhs, EnumType rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(
	        lhs & static_cast<std::underlying_type_t<EnumType>>(rhs)
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> operator ^ (std::underlying_type_t<EnumType> lhs, EnumType rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(
	        lhs ^ static_cast<std::underlying_type_t<EnumType>>(rhs)
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> operator | (EnumType lhs, std::underlying_type_t<EnumType> rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(
	        static_cast<std::underlying_type_t<EnumType>>(lhs) | rhs
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> operator & (EnumType lhs, std::underlying_type_t<EnumType> rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(
	        static_cast<std::underlying_type_t<EnumType>>(lhs) & rhs
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> operator ^ (EnumType lhs, std::underlying_type_t<EnumType> rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(
	        static_cast<std::underlying_type_t<EnumType>>(lhs) ^ rhs
	);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> &operator |= (std::underlying_type_t<EnumType> &lhs, EnumType rhs) {
	return lhs = (lhs | rhs);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> &operator &= (std::underlying_type_t<EnumType> &lhs, EnumType rhs) {
	return lhs = (lhs & rhs);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr std::underlying_type_t<EnumType> &operator ^= (std::underlying_type_t<EnumType> lhs, EnumType rhs) {
	return lhs = (lhs ^ rhs);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr EnumType &operator |= (EnumType lhs, std::underlying_type_t<EnumType> rhs) {
	return lhs = (lhs | rhs);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr EnumType &operator &= (EnumType lhs, std::underlying_type_t<EnumType> rhs) {
	return lhs = (lhs & rhs);
}

template<typename EnumType>
    requires (algorithm::util::EnableEnumPartialOperator<EnumType>::enable)
inline constexpr EnumType &operator ^= (EnumType lhs, std::underlying_type_t<EnumType> rhs) {
	return lhs = (lhs ^ rhs);
}

namespace algorithm::util {

	/*!
	 * @brief To enable partial comparison between enums, just partially specialize this template with enable = true.
	 * @tparam T The type of enumeration
	 */
	template<typename T>
	    requires std::is_enum_v<T>
	struct EnableEnumCompareOperator {
		static constexpr bool enable = false;
	};

}

template<typename EnumType>
	requires(algorithm::util::EnableEnumCompareOperator<EnumType>::enable)
inline constexpr auto operator<=> (EnumType lhs, EnumType rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(lhs) <=> static_cast<std::underlying_type_t<EnumType>>(rhs);
}

namespace algorithm::util {

	/*!
	 * @brief To enable partial comparison between enums, just partially specialize this template with enable = true.
	 * @tparam T The type of enumeration
	 */
	template<typename T>
	    requires std::is_enum_v<T>
	struct EnableEnumComparePartialOperator {
		static constexpr bool enable = false;
	};

}

template<typename EnumType>
    requires(algorithm::util::EnableEnumComparePartialOperator<EnumType>::enable)
inline constexpr bool operator== (EnumType lhs, std::underlying_type_t<EnumType> rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(lhs) == rhs;
}

template<typename EnumType>
    requires(algorithm::util::EnableEnumComparePartialOperator<EnumType>::enable)
inline constexpr auto operator<=> (EnumType lhs, std::underlying_type_t<EnumType> rhs) {
	return static_cast<std::underlying_type_t<EnumType>>(lhs) <=> rhs;
}

template<typename EnumType>
    requires(algorithm::util::EnableEnumComparePartialOperator<EnumType>::enable)
inline constexpr bool operator== (std::underlying_type_t<EnumType> lhs,  EnumType rhs) {
	return lhs == static_cast<std::underlying_type_t<EnumType>>(rhs);
}

template<typename EnumType>
    requires(algorithm::util::EnableEnumComparePartialOperator<EnumType>::enable)
inline constexpr auto operator<=> (std::underlying_type_t<EnumType> lhs,  EnumType rhs) {
	return lhs <=> static_cast<std::underlying_type_t<EnumType>>(rhs);
}

namespace algorithm::util {

	/*!
	 * @brief To enable ostream output between enums, just partially specialize this template with enable = true.
	 * @tparam T The type of enumeration
	 */
	template<typename T>
	    requires std::is_enum_v<T>
	struct EnableEnumOstreamOperator {
		static constexpr bool enable = false;
		static constexpr bool as_underlying_type = false;
	};

	/*!
	 * @brief To enable istream input between enums, just partially specialize this template with enable = true.
	 * @tparam T The type of enumeration
	 */
	template<typename T>
	    requires std::is_enum_v<T>
	struct EnableEnumIstreamOperator {
		static constexpr bool enable = false;
	};
}

#include <istream>
#include <ostream>
#include <reflection/enum.h>

template<typename EnumType>
    requires(algorithm::util::EnableEnumOstreamOperator<EnumType>::enable &&
             algorithm::util::EnableEnumOstreamOperator<EnumType>::as_underlying_type)
inline constexpr std::ostream & operator<< (std::ostream &output_stream, EnumType output) {
	output_stream << static_cast<std::underlying_type_t<EnumType>>(output);
	return output_stream;
}

template<typename EnumType>
    requires(algorithm::util::EnableEnumOstreamOperator<EnumType>::enable &&
             !algorithm::util::EnableEnumOstreamOperator<EnumType>::as_underlying_type)
inline constexpr std::ostream & operator<< (std::ostream &output_stream, EnumType output) {
	output_stream << algorithm::reflect::get_enum_name(output);
	return output_stream;
}

template<typename EnumType>
    requires(algorithm::util::EnableEnumIstreamOperator<EnumType>::enable)
inline constexpr std::istream & operator>> (std::istream &input_stream, EnumType &input) {
	std::underlying_type_t<EnumType> input_temp;
	input_stream >> input_temp;
	input = static_cast<EnumType>(input_temp);
	return input_stream;
}

#endif//ALGORITHM_UTIL_ENUM_OPERATOR_H
