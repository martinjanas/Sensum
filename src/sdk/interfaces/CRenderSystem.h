#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <cstddef>

class CRenderSystem
{
public:
	std::byte pad001[0x170];
	IDXGISwapChain* swap_chain;
};
