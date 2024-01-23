#pragma once

#include <cstdint>
#include "../interfaces/IConVar.hpp"
#include "UtlVector.hpp"
#include "UtlString.hpp"

#define FORCEINLINE_CVAR inline
//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class ConVar;
class CCommand;
class ConCommand;
class CConCommandBase;
struct characterset_t;

class CCommand
{
public:
	CCommand();
	CCommand(int nArgC, const char** ppArgV);
	bool Tokenize(const char* pCommand, characterset_t* pBreakSet = NULL);
	void Reset();

	int ArgC() const;
	const char** ArgV() const;
	const char* ArgS() const;					        // All args that occur after the 0th arg, in string form
	const char* GetCommandString() const;		  // The entire command in string form, including the 0th arg
	const char* operator[](int nIndex) const;	// Gets at arguments
	const char* Arg(int nIndex) const;		      // Gets at arguments

												  // Helper functions to parse arguments to commands.
	const char* FindArg(const char* pName) const;
	int FindArgInt(const char* pName, int nDefaultVal) const;

	static int MaxCommandLength();
	static characterset_t* DefaultBreakSet();

private:
	enum
	{
		COMMAND_MAX_ARGC = 64,
		COMMAND_MAX_LENGTH = 512,
	};

	int		m_nArgc;
	int		m_nArgv0Size;
	char	m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char	m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char* m_ppArgv[COMMAND_MAX_ARGC];
};

inline int CCommand::MaxCommandLength()
{
	return COMMAND_MAX_LENGTH - 1;
}

inline int CCommand::ArgC() const
{
	return m_nArgc;
}

inline const char** CCommand::ArgV() const
{
	return m_nArgc ? (const char**)m_ppArgv : NULL;
}

inline const char* CCommand::ArgS() const
{
	return m_nArgv0Size ? &m_pArgSBuffer[m_nArgv0Size] : "";
}

inline const char* CCommand::GetCommandString() const
{
	return m_nArgc ? m_pArgSBuffer : "";
}

inline const char* CCommand::Arg(int nIndex) const
{
	// FIXME: Many command handlers appear to not be particularly careful
	// about checking for valid argc range. For now, we're going to
	// do the extra check and return an empty string if it's out of range
	if (nIndex < 0 || nIndex >= m_nArgc)
		return "";
	return m_ppArgv[nIndex];
}

inline const char* CCommand::operator[](int nIndex) const
{
	return Arg(nIndex);
}

//-----------------------------------------------------------------------------
// Any executable that wants to use ConVars need to implement one of
// these to hook up access to console variables.
//-----------------------------------------------------------------------------
class IConCommandBaseAccessor
{
public:
	// Flags is a combination of FCVAR flags in cvar.h.
	// hOut is filled in with a handle to the variable.
	virtual bool RegisterConCommandBase(CConCommandBase* pVar) = 0;
};

//-----------------------------------------------------------------------------
// Called when a ConCommand needs to execute
//-----------------------------------------------------------------------------
typedef void(*FnCommandCallbackV1_t)(void);
typedef void(*FnCommandCallback_t)(const CCommand& command);

#define COMMAND_COMPLETION_MAXITEMS       64
#define COMMAND_COMPLETION_ITEM_LENGTH    64

//-----------------------------------------------------------------------------
// Returns 0 to COMMAND_COMPLETION_MAXITEMS worth of completion strings
//-----------------------------------------------------------------------------
typedef int(*FnCommandCompletionCallback)(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);

//-----------------------------------------------------------------------------
// Interface version
//-----------------------------------------------------------------------------
class ICommandCallback
{
public:
	virtual void CommandCallback(const CCommand& command) = 0;
};

class ICommandCompletionCallback
{
public:
	virtual int  CommandCompletionCallback(const char* pPartial, CUtlVector<CUtlString>& commands) = 0;
};
enum CVarDLLIdentifier_t;
//-----------------------------------------------------------------------------
// Purpose: The base console invoked command/cvar interface
//-----------------------------------------------------------------------------
class CConCommandBase
{
public:
	virtual ~CConCommandBase() { }

	bool IsCommand()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 1)(this);
	}

	bool IsFlagSet(int nFlag)
	{
		return CallVFunction<bool(__thiscall*)(void*, int)>(this, 2)(this, nFlag);
	}

	void AddFlags(int nFlag)
	{
		CallVFunction<void(__thiscall*)(void*, int)>(this, 3)(this, nFlag);
	}

	void RemoveFlags(int nFlag)
	{
		CallVFunction<void(__thiscall*)(void*, int)>(this, 4)(this, nFlag);
	}

	int GetFlags()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, 5)(this);
	}

	const char* GetName()
	{
		//    @ida ConVar::GetName(): client.dll -> "8B 41 1C 8B 40 0C"
		return CallVFunction<const char*(__thiscall*)(void*)>(this, 6)(this);
	}

	const char* GetHelpText()
	{
		return CallVFunction<const char* (__thiscall*)(void*)>(this, 7)(this);
	}

	bool IsRegistered()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 8)(this);
	}

public:
	CConCommandBase* pNext; // 0x04
	std::uint32_t bRegistered : 1; // 0x08 // @ida: engine.dll -> U8["8B 40 ? 83 E0 01 C3" + 0x2] |    @ida: vstdlib.dll -> U8["83 4E ? 01 FF 50 18 85 C0 0F" + 0x2]
	std::uint32_t nOldFlags : 31; // 0x08 //    @Note: used to detect convars modification, added since 22.10.2022
	const char* szName; // 0x0C
	const char* szHelpText; // 0x10
	int nFlags; // 0x14
	FnCommandCallbackV1_t pCallback; //0x18
};
static_assert(sizeof(CConCommandBase) == 0x1C);

//-----------------------------------------------------------------------------
// Purpose: The console invoked command
//-----------------------------------------------------------------------------
class ConCommand : public CConCommandBase
{
	friend class CCvar;

public:
	typedef CConCommandBase BaseClass;

	ConCommand(const char* pName, FnCommandCallbackV1_t callback,
		const char* pHelpString = 0, int flags = 0, FnCommandCompletionCallback completionFunc = 0);
	ConCommand(const char* pName, FnCommandCallback_t callback,
		const char* pHelpString = 0, int flags = 0, FnCommandCompletionCallback completionFunc = 0);
	ConCommand(const char* pName, ICommandCallback* pCallback,
		const char* pHelpString = 0, int flags = 0, ICommandCompletionCallback * pCommandCompletionCallback = 0);

	virtual         ~ConCommand(void);
	virtual bool    IsCommand(void) const;
	virtual int     AutoCompleteSuggest(const char* partial, CUtlVector<CUtlString>& commands);
	virtual bool    CanAutoComplete(void);
	virtual void    Dispatch(const CCommand& command);

	//private:
	// NOTE: To maintain backward compat, we have to be very careful:
	// All public virtual methods must appear in the same order always
	// since engine code will be calling into this code, which *does not match*
	// in the mod code; it's using slightly different, but compatible versions
	// of this class. Also: Be very careful about adding new fields to this class.
	// Those fields will not exist in the version of this class that is instanced
	// in mod code.

	// Call this function when executing the command
	union
	{
		FnCommandCallbackV1_t       m_fnCommandCallbackV1;
		FnCommandCallback_t         m_fnCommandCallback;
		ICommandCallback* m_pCommandCallback;
	};

	union
	{
		FnCommandCompletionCallback m_fnCompletionCallback;
		ICommandCompletionCallback* m_pCommandCompletionCallback;
	};

	bool m_bHasCompletionCallback : 1;
	bool m_bUsingNewCommandCallback : 1;
	bool m_bUsingCommandCallbackInterface : 1;
};

//-----------------------------------------------------------------------------
// Purpose: A console variable
//-----------------------------------------------------------------------------
class ConVar : public CConCommandBase
{
public:
	virtual ~ConVar() { }

	float GetFloat()
	{
		//    @ida ConVar::GetFloat(): client.dll -> "55 8B EC 51 56 8B F1 8B 4E 1C 3B CE 75 4C"

#ifdef Q_ALLOW_VIRTUAL_REBUILD
		Q_ASSERT(value.flValue == oldValue.flValue && value.iValue == oldValue.iValue && (nFlags ^ nOldFlags) == 0); // you triggered an possible detection vector; use virtual calls to set values instead

		std::uint32_t uObscure = *reinterpret_cast<std::uint32_t*>(&pParent->value.flValue) ^ reinterpret_cast<std::intptr_t>(this);
		return *reinterpret_cast<float*>(&uObscure);
#else
		return CallVFunction<float(__thiscall*)(void*)>(this, 12)(this);
#endif
	}

	int GetInt()
	{
		//    @ida ConVar::GetInt(): client.dll -> "56 8B F1 8B 4E 1C 3B CE 75 3E"

#ifdef Q_ALLOW_VIRTUAL_REBUILD
		Q_ASSERT(value.flValue == oldValue.flValue && value.iValue == oldValue.iValue && (nFlags ^ nOldFlags) == 0); // you triggered an possible detection vector; use virtual calls to set values instead

		return pParent->value.iValue ^ reinterpret_cast<std::intptr_t>(this);
#else
		return CallVFunction<int(__thiscall*)(void*)>(this, 13)(this);
#endif
	}

	bool GetBool()
	{
		return !!GetInt();
	}

	const char* GetString() const
	{
		if (nFlags & FCVAR_NEVER_AS_STRING)
			return "FCVAR_NEVER_AS_STRING";

		const char* szValue = pParent->value.szString;
		return szValue != nullptr ? szValue : "";
	}

	void SetValue(const char* szNewValue)
	{
		return CallVFunction<void(__thiscall*)(void*, const char*)>(this, 14)(this, szNewValue);
	}

	void SetValue(float flNewValue)
	{
		return CallVFunction<void(__thiscall*)(void*, float)>(this, 15)(this, flNewValue);
	}

	void SetValue(int iNewValue)
	{
		return CallVFunction<void(__thiscall*)(void*, int)>(this, 16)(this, iNewValue);
	}

	void SetValue(Color colNewValue)
	{
		return CallVFunction<void(__thiscall*)(void*, Color)>(this, 17)(this, colNewValue);
	}

public:
	struct CVValue_t
	{
		char* szString; // 0x00
		int nStringLength; // 0x04
		float flValue; // 0x08
		int iValue; // 0x0C
	};

	ConVar* pParent; // 0x1C
	const char* szDefaultValue; // 0x20
	CVValue_t value; // 0x24
	CVValue_t oldValue; // 0x34 //    @Note: used to detect convars modification, added since 22.10.2022
	bool bHasMin; // 0x44
	float flMinValue; // 0x48
	bool bHasMax; // 0x4C
	float flMaxValue; // 0x50
	CUtlVector<FnChangeCallback_t> m_fnChangeCallbacks; // 0x54 //    @ida: engine.dll -> U8["8B 4B ? 39 31 74 40 40" + 0x2]
};
static_assert(sizeof(ConVar) == 0x68);

//-----------------------------------------------------------------------------
// Called by the framework to register ConCommands with the ICVar
//-----------------------------------------------------------------------------
void ConVar_Register(int nCVarFlag = 0, IConCommandBaseAccessor * pAccessor = NULL);
void ConVar_Unregister();