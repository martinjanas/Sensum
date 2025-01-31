#include "IconFetcher.h"

#include "Icons.h"
#include "utils.h"

namespace icon_fetcher
{
    std::unordered_map<EIconType, icon_data_t> icon_data;

    void populate_data(EIconType icon_type, const uint8_t* data, size_t size, int width, int height, ID3D11Device* device)
    {
        icon_data_t temp;
        temp.w = width;
        temp.h = height;
        temp.texture = utils::create_image_texture(data, size, &temp.w, &temp.h, device);

        icon_data[icon_type] = temp;
    }
    
    void fetch_icon_data(ID3D11Device* device)
    {
        populate_data(EIconType::SMOKE, icons::smokegrenade, sizeof(icons::smokegrenade), 15, 32, device);
        populate_data(EIconType::MOLOTOV, icons::molotov, sizeof(icons::molotov), 19, 32, device);
        populate_data(EIconType::HEGRENADE, icons::hegrenade, sizeof(icons::hegrenade), 19, 32, device);
        populate_data(EIconType::FLASHBANG, icons::flashbang, sizeof(icons::flashbang), 30, 32, device);
        populate_data(EIconType::DECOY, icons::decoy, sizeof(icons::decoy), 30, 32, device);
    }

    icon_data_t get(const EIconType& type)
    {
        return icon_data[type];
    }
}
