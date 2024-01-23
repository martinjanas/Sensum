#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <map>
#include "misc/Recv.hpp"
#include "../helpers//fnv.h"

#define PNETVAR_OFFSET(type, funcname, class_name, var_name, offset) \
auto funcname() -> std::add_pointer_t<type> \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	const auto addr = std::uintptr_t(this) + offset + netvar_manager::get_offset_by_hash_cached<hash>(); \
	return reinterpret_cast<std::add_pointer_t<type>>(addr); \
}

#define PNETVAR(type, funcname, class_name, var_name) \
	PNETVAR_OFFSET(type, funcname, class_name, var_name, 0)

#define NETVAR_OFFSET(type, funcname, class_name, var_name, offset) \
auto funcname() -> std::add_lvalue_reference_t<type> \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	const auto addr = std::uintptr_t(this) + offset + netvar_manager::get_offset_by_hash_cached<hash>(); \
	return *reinterpret_cast<std::add_pointer_t<type>>(addr); \
}

#define NETVAR(type, funcname, class_name, var_name) \
	NETVAR_OFFSET(type, funcname, class_name, var_name, 0)

#define NETPROP(funcname, class_name, var_name) \
static auto funcname() ->  RecvProp* \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	static auto prop_ptr = netvar_manager::get().get_prop(hash); \
	return prop_ptr; \
}

class netvar_manager
{
private:
	struct stored_data
	{
		RecvProp* prop_ptr;
		uint32_t class_relative_offset;
	};

public:
	// Return a const instance, others shouldnt modify this.
	static auto get() -> const netvar_manager &
	{
		static netvar_manager instance;
		return instance;
	}

	auto get_offset(const fnv::hash hash) const -> std::uint32_t
	{
		return m_props.at(hash).class_relative_offset;
	}

	auto get_prop(const fnv::hash hash) const -> RecvProp*
	{
		return m_props.at(hash).prop_ptr;
	}

	__declspec(noinline) static auto get_offset_by_hash(const fnv::hash hash) -> std::uint32_t
	{
		return get().get_offset(hash);
	}

	template<fnv::hash Hash>
	static auto get_offset_by_hash_cached() -> std::uint32_t
	{
		static auto offset = std::uint32_t(0);
		if (!offset)
			offset = get_offset_by_hash(Hash);
		return offset;
	}

private:
	netvar_manager();
	auto dump_recursive(const char* base_class, RecvTable* table, std::uint32_t offset) -> void;

private:
	std::map<fnv::hash, stored_data> m_props;
};