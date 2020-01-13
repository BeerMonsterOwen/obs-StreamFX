/*
 * Modern effects for a modern Streamer
 * Copyright (C) 2018 Michael Fabian Dirks
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#pragma once
#include <cinttypes>
#include <limits>
#include <type_traits>

extern "C" {
#include <obs-config.h>
#include <obs.h>
}

const char* obs_module_recursive_text(const char* to_translate, size_t depth = std::numeric_limits<size_t>::max());

template<typename Enum>
struct enable_bitmask_operators {
	static const bool enable = false;
};

template<typename Enum>
typename std::enable_if<enable_bitmask_operators<Enum>::enable, Enum>::type operator|(Enum lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename Enum>
typename std::enable_if<enable_bitmask_operators<Enum>::enable, Enum>::type operator&(Enum lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename Enum>
typename std::enable_if<enable_bitmask_operators<Enum>::enable, bool>::type any(Enum lhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<underlying>(lhs) != static_cast<underlying>(0);
}

template<typename Enum>
typename std::enable_if<enable_bitmask_operators<Enum>::enable, bool>::type exact(Enum lhs, Enum rhs)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<underlying>(lhs) == static_cast<underlying>(rhs);
}

#define P_ENABLE_BITMASK_OPERATORS(x)    \
	template<>                           \
	struct enable_bitmask_operators<x> { \
		static const bool enable = true; \
	};

#define D_STR(s) #s
#define D_VSTR(s) D_STR(s)

namespace util {
	bool inline are_property_groups_broken()
	{
		return obs_get_version() < MAKE_SEMANTIC_VERSION(24, 0, 0);
	}

	struct obs_graphics {
		obs_graphics()
		{
			obs_enter_graphics();
		}
		~obs_graphics()
		{
			obs_leave_graphics();
		}
	};

	obs_property_t* obs_properties_add_tristate(obs_properties_t* props, const char* name, const char* desc);

	inline bool is_tristate_enabled(int64_t tristate)
	{
		return tristate == 1;
	}

	inline bool is_tristate_disabled(int64_t tristate)
	{
		return tristate == 0;
	}

	inline bool is_tristate_default(int64_t tristate)
	{
		return tristate == -1;
	}

	typedef union {
		uint32_t color;
		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
	} rgba32;
	typedef union {
		uint32_t color;
		struct {
			uint8_t a;
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};
	} argb32;
} // namespace util
