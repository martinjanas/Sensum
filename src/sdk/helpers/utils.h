#pragma once
#include <cstdint>
#include <d3d11.h>

namespace utils
{
	const char* hitbox_index_to_name(int index);
	ID3D11ShaderResourceView* create_image_texture(const uint8_t* data, size_t size, int* out_width, int* out_height, ID3D11Device* device);
}

