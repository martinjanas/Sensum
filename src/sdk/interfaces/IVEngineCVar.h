#pragma once
#include <cstddef>
#include "../helpers/CUtlLinkedList.h"
#include "../math/Vector.h"
#include "../math/QAngle.h"
#include "../helpers/fnv.h"

#define FCVAR_NONE 0
#define FCVAR_UNREGISTERED (1 << 0)	// if this is set, don't add to linked list, etc
#define FCVAR_DEVELOPMENTONLY (1 << 1)	// hidden in released products. flag is removed automatically if allow_development_cvars is defined
#define FCVAR_GAMEDLL (1 << 2)	// defined by the game dll
#define FCVAR_CLIENTDLL (1 << 3)	// defined by the client dll
#define FCVAR_HIDDEN (1 << 4)	// hidden. doesn't appear in find or autocomplete. like developmentonly, but can't be compiled out

#define FCVAR_PROTECTED (1 << 5)	// it's a server cvar, but we don't send the data since it's a password, etc. sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY (1 << 6)	// this cvar cannot be changed by clients connected to a multiplayer server
#define FCVAR_ARCHIVE (1 << 7)	// set to cause it to be saved to vars.rc
#define FCVAR_NOTIFY (1 << 8)	// notifies players when changed
#define FCVAR_USERINFO (1 << 9)	// changes the client's info string
#define FCVAR_CHEAT (1 << 14)	// only useable in singleplayer/debug/multiplayer & sv_cheats
#define FCVAR_PRINTABLEONLY (1 << 10)	// this cvar's string cannot contain unprintable characters (e.g., used for player name etc)
#define FCVAR_UNLOGGED (1 << 11)	// if this is a fcvar_server, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING (1 << 12)	// never try to print that cvar

#define FCVAR_REPLICATED (1 << 13)	// server setting enforced on clients, replicated
#define FCVAR_MODIFIED (1 << 14) //used by the game, probably used as modification detection
#define FCVAR_DEMO (1 << 16)	// record this cvar when starting a demo file
#define FCVAR_DONTRECORD (1 << 17)	// don't record these command in demofiles
#define FCVAR_RELOAD_MATERIALS (1 << 20)	// if this cvar changes, it forces a material reload
#define FCVAR_RELOAD_TEXTURES (1 << 21)	// if this cvar changes, if forces a texture reload
#define FCVAR_NOT_CONNECTED (1 << 22)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_MATERIAL_SYSTEM_THREAD (1 << 23)	// indicates this cvar is read from the material system thread
#define FCVAR_ARCHIVE_XBOX (1 << 24)	// cvar written to config.cfg on the xbox
#define FCVAR_ACCESSIBLE_FROM_THREADS (1 << 25)	// used as a debugging tool necessary to check material system thread convars
#define FCVAR_SERVER_CAN_EXECUTE (1 << 28)	// the server is allowed to execute this command on clients via clientcommand/net_stringcmd/cbaseclientstate::processstringcmd
#define FCVAR_SERVER_CANNOT_QUERY (1 << 29)	// if this is set, then the server is not allowed to query this cvar's value (via iserverpluginhelpers::startquerycvarvalue)
#define FCVAR_CLIENTCMD_CAN_EXECUTE (1 << 30)	// ivengineclient::clientcmd is allowed to execute this command
#define FCVAR_MATERIAL_THREAD_MASK (FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD)

enum EConVarType : short
{
	EConVarType_Invalid = -1,
	EConVarType_Bool,
	EConVarType_Int16,
	EConVarType_UInt16,
	EConVarType_Int32,
	EConVarType_UInt32,
	EConVarType_Int64,
	EConVarType_UInt64,
	EConVarType_Float32,
	EConVarType_Float64,
	EConVarType_String,
	EConVarType_Color,
	EConVarType_Vector2,
	EConVarType_Vector3,
	EConVarType_Vector4,
	EConVarType_Qangle,
	EConVarType_MAX
};

union CVValue_t
{
	bool as_bool;
	short as_short;
	uint16_t as_ushort;
	int as_int;
	uint32_t as_uint;
	int64_t as_long;
	uint64_t as_ulong;
	float as_float;
	double as_double;
	const char* as_str;
	float as_color[4];
	Vector as_vec2;
	Vector as_vec3;
	void* as_vec4;
	QAngle as_qangle;
};

class Convar
{
public:
	const char* szName; // 0x0000
	Convar* m_pNext; // 0x0008
	std::byte pad01[0x10]; //0x0010
	const char* szDescription; // 0x0020
	uint32_t nType; // 0x28
	uint32_t nRegistered; // 0x2C
	uint32_t nFlags; // 0x30
	std::byte pad02[0x8]; //0x34
	CVValue_t value; // 0x40 //read-only, mofify with caution
};

class IVEngineCVar
{
public:
	std::byte pad01[0x40];
	CUtlLinkedList<Convar*> convars;

	Convar* find(const fnv::hash& hash)
	{
		for (auto& convar : convars)
		{
			if (!convar)
				continue;

			auto name_hashed = fnv::hash_runtime(convar->szName);
			if (hash == name_hashed)
				return convar;
		}
	}
};