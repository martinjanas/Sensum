#include "utils.h"
#include "../helpers/Hitbox_t.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../thirdparty/ImGui/stb_image.h"
#include "../hooking/safetyhook/Zydis.h"

namespace utils
{
    const char* hitbox_index_to_name(int index)
    {
        switch (index)
        {
        case HITBOX_HEAD:
            return "Head";
        case HITBOX_NECK:
            return "Neck";
        case HITBOX_PELVIS:
            return "Pelvis";
        case HITBOX_BELLY:
            return "Belly";
        case HITBOX_THORAX:
            return "Thorax";
        case HITBOX_LOWER_CHEST:
            return "Lower Chest";
        case HITBOX_UPPER_CHEST:
            return "Upper Chest";
        case HITBOX_RIGHT_THIGH:
            return "Right Thigh";
        case HITBOX_LEFT_THIGH:
            return "Left Thigh";
        case HITBOX_RIGHT_CALF:
            return "Right Calf";
        case HITBOX_LEFT_CALF:
            return "Left Calf";
        case HITBOX_RIGHT_FOOT:
            return "Right Foot";
        case HITBOX_LEFT_FOOT:
            return "Left Foot";
        case HITBOX_RIGHT_HAND:
            return "Right Hand";
        case HITBOX_LEFT_HAND:
            return "Left Hand";
        case HITBOX_RIGHT_UPPER_ARM:
            return "Right Upper Arm";
        case HITBOX_RIGHT_FOREARM:
            return "Right Forearm";
        case HITBOX_LEFT_UPPER_ARM:
            return "Left Upper Arm";
        case HITBOX_LEFT_FOREARM:
            return "Left Forearm";
        }
    }
    
    ID3D11ShaderResourceView* create_image_texture(const uint8_t* data, size_t size, int* out_width, int* out_height, ID3D11Device* device)
    {
        auto create_texture_from_image = [&](const uint8_t* image_data, int width, int height) -> ID3D11ShaderResourceView* 
        {
            D3D11_TEXTURE2D_DESC texture_desc = {};
            texture_desc.Width = width;
            texture_desc.Height = height;
            texture_desc.MipLevels = 1;
            texture_desc.ArraySize = 1;
            texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            texture_desc.SampleDesc.Count = 1;
            texture_desc.Usage = D3D11_USAGE_DEFAULT;
            texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

            D3D11_SUBRESOURCE_DATA init_data = {};
            init_data.pSysMem = image_data;
            init_data.SysMemPitch = width * 4;

            ID3D11Texture2D* texture = nullptr;
            HRESULT hr = device->CreateTexture2D(&texture_desc, &init_data, &texture);
            if (FAILED(hr))
                return nullptr;

            ID3D11ShaderResourceView* srv = nullptr;
            D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
            srv_desc.Format = texture_desc.Format;
            srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MipLevels = 1;

            hr = device->CreateShaderResourceView(texture, &srv_desc, &srv);
            texture->Release();
            if (FAILED(hr))
                return nullptr;

            return srv;
        };


        int channels;
        uint8_t* pixels = stbi_load_from_memory(data, (int)size, out_width, out_height, &channels, 4);
        if (!pixels)
            return nullptr;
        
        ID3D11ShaderResourceView* srv = create_texture_from_image(pixels, *out_width, *out_height);
        stbi_image_free(pixels);

        return srv;
    }
    
    uint8_t* calculate_hook_ip(void* target)
        {
            ZydisDecoder decoder;
            ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

            auto ip = reinterpret_cast<uint8_t*>(target);

            while (*ip != 0xC3)
            {
                ZydisDecodedInstruction ix{};
                ZydisDecoderDecodeInstruction(&decoder, nullptr, reinterpret_cast<void*>(ip), 15, &ix);

                // Follow JMPs
                if (ix.opcode == 0xE9) 
                    ip += ix.length + (int32_t)ix.raw.imm[0].value.s;
                else ip += ix.length;
            }

            return ip;
        }
    
}
