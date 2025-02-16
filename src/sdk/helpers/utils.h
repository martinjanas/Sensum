#pragma once
#include <cstdint>
#include <d3d11.h>
#include <type_traits>
#include "../hooking/safetyhook/safetyhook.hpp"

namespace utils
{
	const char* hitbox_index_to_name(int index);
	ID3D11ShaderResourceView* create_image_texture(const uint8_t* data, size_t size, int* out_width, int* out_height, ID3D11Device* device);
	uint8_t* calculate_hook_ip(void* target);
	
	template <typename T, int level>
	T get_context_argument(SafetyHookContext* ctx)
	{
		if constexpr (level == 1) {
			if constexpr (std::is_pointer_v<T>) 
				return reinterpret_cast<T>(ctx->rcx); 
			else 
				return static_cast<T>(ctx->rcx);     
		}

		if constexpr (level == 2) {
			if constexpr (std::is_pointer_v<T>) 
				return reinterpret_cast<T>(ctx->rdx);
			else 
				return static_cast<T>(ctx->rdx);
		}

		if constexpr (level == 3) {
			if constexpr (std::is_pointer_v<T>) 
				return reinterpret_cast<T>(ctx->r8);
			else 
				return static_cast<T>(ctx->r8);
		}

		if constexpr (level == 4) {
			if constexpr (std::is_pointer_v<T>) 
				return reinterpret_cast<T>(ctx->r9);
			else 
				return static_cast<T>(ctx->r9);
		}

		// Arguments beyond the 4th are on the stack (rsp + 0x28 + ((level - 5) * 8))
		uint8_t* address = reinterpret_cast<uint8_t*>(ctx->rsp + 0x28 + ((level - 5) * 8));

		if constexpr (std::is_pointer_v<T>) 
			return *reinterpret_cast<T*>(address);
		else 
			return *reinterpret_cast<T*>(address);
	}

	template <typename T, int level>
	void set_context_argument(SafetyHookContext* ctx, T value)
	{
		if constexpr (level == 1) 
		{
			if constexpr (std::is_pointer_v<T>) 
				ctx->rcx = reinterpret_cast<uintptr_t>(value); // Set rcx register to value
			else 
				ctx->rcx = static_cast<uintptr_t>(value); // Set rcx register to value     
		}
		else if constexpr (level == 2) 
		{
			if constexpr (std::is_pointer_v<T>) 
				ctx->rdx = reinterpret_cast<uintptr_t>(value); // Set rdx register to value
			else 
				ctx->rdx = static_cast<uintptr_t>(value); // Set rdx register to value
		}
		else if constexpr (level == 3) 
		{
			if constexpr (std::is_pointer_v<T>) 
				ctx->r8 = reinterpret_cast<uintptr_t>(value); // Set r8 register to value
			else 
				ctx->r8 = static_cast<uintptr_t>(value); // Set r8 register to value
		}
		else if constexpr (level == 4) 
		{
			if constexpr (std::is_pointer_v<T>) 
				ctx->r9 = reinterpret_cast<uintptr_t>(value); // Set r9 register to value
			else 
				ctx->r9 = static_cast<uintptr_t>(value); // Set r9 register to value
		}
		else 
		{
			// Arguments beyond the 4th are on the stack (trampoline_rsp + 0x28 + ((level - 5) * 8))
			uint8_t* address = reinterpret_cast<uint8_t*>(ctx->trampoline_rsp + 0x28 + ((level - 5) * 8));

			if constexpr (std::is_pointer_v<T>) 
				*reinterpret_cast<T*>(address) = value; // Set stack-based argument (pointer type)
			else 
				*reinterpret_cast<T*>(address) = value; // Set stack-based argument (non-pointer type)
		}
	}
}

