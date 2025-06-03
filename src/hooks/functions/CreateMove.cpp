#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"
#include "../../sdk/helpers/CUtlBuffer.h"
/*
    MJ: Hooking two createmoves now, because aimbot & rcs is behaving weirdly in createmove_csgoinput21

    TODO: Fix crc?
*/

struct cmd_data_t
{
    CInButtonStatePB buttons;
    QAngle view;
} static inline cmd_data;

void apply_cmd(CUserCmd* cmd)
{
    const auto& base_cmd = cmd->csgoUserCmd.pBaseCmd;
    if (base_cmd)
    {
        base_cmd->pInButtonState->nValue = cmd_data.buttons.nValue; //doesnt make a difference anyway, pInButtonState doesnt work, do cmd one aswell
        base_cmd->pInButtonState->nValueChanged = cmd_data.buttons.nValueChanged;
        base_cmd->pInButtonState->nValueScroll = cmd_data.buttons.nValueScroll;

        if (base_cmd->pViewAngles)
            base_cmd->pViewAngles->angValue = cmd_data.view;
    }
}

void save_cmd(CUserCmd* cmd)
{
    const auto& base_cmd = cmd->csgoUserCmd.pBaseCmd;
    if (base_cmd)
    {
        cmd_data.buttons.nValue = base_cmd->pInButtonState->nValue;
        cmd_data.buttons.nValueChanged = base_cmd->pInButtonState->nValueChanged;
        cmd_data.buttons.nValueScroll = base_cmd->pInButtonState->nValueScroll;

        if (base_cmd->pViewAngles)
            cmd_data.view = base_cmd->pViewAngles->angValue;
    }
}

bool SerializePartialToArray(CBaseUserCmdPB* base_cmd, const CUtlBuffer& buffer, int crc_size)
{
    using fn = bool(__thiscall*)(void*, const CUtlBuffer&, int);
    static auto hash = xxh::get_hash("SerializePartialToArray");
    static const auto& addr = modules::client.get_sig(hash, __FUNCTION__).as();
    if (!addr)
        return false;

    auto serialize_partial_to_array = reinterpret_cast<fn>(addr);
    if (serialize_partial_to_array)
        return serialize_partial_to_array(base_cmd, buffer, crc_size);

    return false;
}

void WriteMessage(void* msg, const CUtlBuffer& buffer, int crc_size)
{
    using fn = void(__fastcall*)(void*, const CUtlBuffer&, int);
    static auto hash = xxh::get_hash("WriteMessage");
    static const auto& addr = modules::client.get_sig(hash, __FUNCTION__).as();
    if (!addr)
        return;

    auto write_message = reinterpret_cast<fn>(addr);
    if (write_message)
        write_message(msg, buffer, crc_size);
}

std::string* SetMessageData(void* move_crc, void* msg, void* nHasBits)
{
    using fn = std::string* (__fastcall*)(void*, void*, void*);
    static auto hash = xxh::get_hash("SetMessageData");
    static const auto& addr = modules::client.get_sig(hash, __FUNCTION__).as();
    if (!addr)
        return nullptr;

    auto set_message_data = reinterpret_cast<fn>(addr);
    if (set_message_data)
        return set_message_data(move_crc, msg, nHasBits);

    return nullptr;
}

void* Alloc(std::size_t size)
{
    using fn = void* (__thiscall*)(std::size_t);
    static const auto& addr = modules::tier0.get_export("MemAlloc_AllocFunc").as();
    if (!addr)
        return nullptr;

    auto alloc = reinterpret_cast<fn>(addr);
    if (alloc)
        return alloc(size);

    return nullptr;
}

void Free(const void* p)
{
    using fn = void(__thiscall*)(const void*);
    static const auto& addr = modules::tier0.get_export("MemAlloc_FreeFunc").as();
    if (!addr)
        return;

    auto free = reinterpret_cast<fn>(addr);
    if (free)
        free(p);
}

bool CalculateCRC(CBaseUserCmdPB* base_cmd) //CCSGOUserCmdPB
{
    CUtlBuffer buffer(0, 0, 0);
    auto crc_size = base_cmd->CalculateCmdCRCSize();
    
    buffer.EnsureCapacity(crc_size + 1);

    const bool serialize_result = SerializePartialToArray(base_cmd, buffer, crc_size);
    if (serialize_result)
    {
        //uintptr_t* msg = static_cast<uintptr_t*>(g::mem_alloc->Alloc(0x18));
        //void* msg{};
        void* msg = Alloc(0x18);

        base_cmd->nCachedBits |= 1;

        auto has_bits = (base_cmd->nHasBits & 0xFFFFFFFFFFFFFFFC);
        if ((has_bits & 1) != 0)
            has_bits = static_cast<uint32_t>(has_bits);
        
        WriteMessage(msg, buffer, crc_size);
        base_cmd->strMoveCrc = SetMessageData((void*)&base_cmd->strMoveCrc, msg, &has_bits);
        Free(msg);

        return true;
    }
    return false; //stack around buffer was corrupted
}

static inline CUserCmd* g_cmd = nullptr;

void __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, bool active)
{
    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        original_fn(rcx, slot, active);
    
    entity_data::fetch_player_data();
    entity_data::fetch_entity_info();
    
    if (g_cmd && g_cmd->csgoUserCmd.pBaseCmd && g_cmd->csgoUserCmd.pBaseCmd->nWeaponSelect == 0)
    {
        features::aimbot::handle(g_cmd);
    }

    original_fn(rcx, slot, active);
}

bool __fastcall hooks::createmove_csgoinput21::hooked(void* rcx, int slot, CUserCmd* cmd, float curtime)
{
    g_cmd = cmd;

    features::bhop::handle(cmd);

    if (cmd && cmd->csgoUserCmd.pBaseCmd)
    {
        /*if (g::mat_system)
        {
            IMaterial** material = nullptr;
            static auto mat = g::mat_system->FindMaterial(&material, "materials/dev/primary_white.vmat");

            if (material)
            {
                IMaterial* mat2 = *material;

                if (mat2)
                {
                    g_Console->println("name: %s, %s", mat2->GetName(), mat2->GetShareName());
                }
            }
        }*/

        /*save_cmd(cmd);
        if (CalculateCRC(cmd->csgoUserCmd.pBaseCmd))
            apply_cmd(cmd);*/
    }

    return original_fn(rcx, slot, cmd, curtime);
}
