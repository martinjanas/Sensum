#pragma once

class IConsoleDisplayFunc
{
public:
    virtual void ColorPrint(const uint8_t* clr, const char* pMessage) = 0;
    virtual void Print(const char* pMessage) = 0;
    virtual void DPrint(const char* pMessage) = 0;
};

class ICvarQuery;

class ICvar : public IAppSystem
{
public:
    virtual CVarDLLIdentifier_t        AllocateDLLIdentifier() = 0; // 9
    virtual void                       RegisterConCommand(CConCommandBase* pCommandBase) = 0; //10
    virtual void                       UnregisterConCommand(CConCommandBase* pCommandBase) = 0;
    virtual void                       UnregisterConCommands(CVarDLLIdentifier_t id) = 0;
    virtual const char*                GetCommandLineValue(const char* pVariableName) = 0;
    virtual CConCommandBase*            FindCommandBase(const char* name) = 0;
    virtual const CConCommandBase*      FindCommandBase(const char* name) const = 0;
    virtual ConVar*                    find(const char* var_name) = 0; //16
    virtual const ConVar*              find(const char* var_name) const = 0;
    virtual ConCommand*                FindCommand(const char* name) = 0;
    virtual const ConCommand*          FindCommand(const char* name) const = 0;
    virtual void                       InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
    virtual void                       RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
    virtual void                       call_callbacks(ConVar* var, const char* pOldString, float flOldValue) = 0;
    virtual void                       InstallConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
    virtual void                       RemoveConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
    virtual void                       ConsoleColorPrintf(const Color& clr, const char* pFormat, ...) const = 0;
    virtual void                       ConsolePrintf(const char* pFormat, ...) const = 0;
    virtual void                       ConsoleDPrintf(const char* pFormat, ...) const = 0;
    virtual void                       RevertFlaggedConVars(int nFlag) = 0;

    //New virtual functions beginning:
    virtual void			           InstallCVarQuery(ICvarQuery* pQuery) = 0;
	virtual void			           SetMaxSplitScreenSlots(int nSlots) = 0;
	virtual int				           GetMaxSplitScreenSlots() const = 0;

	virtual void			           AddSplitScreenConVars() = 0;
	virtual void			           RemoveSplitScreenConVars(CVarDLLIdentifier_t id) = 0;

	virtual int				           GetConsoleDisplayFuncCount() const = 0;
	virtual void			           GetConsoleText(int nDisplayFuncIndex, char* pchText, size_t bufSize) const = 0;

	// Utilities for convars accessed by the material system thread
	virtual bool			           IsMaterialThreadSetAllowed() const = 0;
	virtual void			           QueueMaterialThreadSetValue(ConVar* pConVar, const char* pValue) = 0;
	virtual void			           QueueMaterialThreadSetValue(ConVar* pConVar, int nValue) = 0;
	virtual void			           QueueMaterialThreadSetValue(ConVar* pConVar, float flValue) = 0;
	virtual bool			           HasQueuedMaterialThreadConVarSets() const = 0;
	virtual int			               ProcessQueuedMaterialThreadConVarSets() = 0;

protected:	class ICVarIteratorInternal;
public:
	
	class Iterator
	{
	public:
		inline Iterator(ICvar* icvar)
		{
			m_pIter = icvar->FactoryInternalIterator();
		}

		inline ~Iterator()
		{
			delete m_pIter;
		}

		inline void SetFirst() RESTRICT
		{
			m_pIter->SetFirst();
		}

		inline void Next() RESTRICT
		{
			m_pIter->Next();
		}

		inline bool IsValid() RESTRICT
		{
			return m_pIter->IsValid();
		}

		inline CConCommandBase* Get() RESTRICT
		{
			return m_pIter->Get();
		}
	private:
		ICVarIteratorInternal* m_pIter;
	};

protected:
	// internals for  ICVarIterator
	class ICVarIteratorInternal
	{
	public:
		virtual void			SetFirst() RESTRICT = 0;
		virtual void			Next() RESTRICT = 0;
		virtual	bool			IsValid() RESTRICT = 0;
		virtual CConCommandBase* Get() RESTRICT = 0;
	};

	virtual ICVarIteratorInternal* FactoryInternalIterator(void) = 0;
	friend class Iterator;
};