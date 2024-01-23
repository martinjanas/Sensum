#pragma once

class IClientUnknown;
class ClientClass;
class bf_read;

class IClientNetworkable
{
public:
	virtual IClientUnknown*			GetIClientUnknown() = 0;
	virtual void					Release() = 0;
	virtual ClientClass*			GetClientClass() = 0; //2
	virtual void					NotifyShouldTransmit(int state) = 0;
	virtual void					OnPreDataChanged(int updateType) = 0;
	virtual void					OnDataChanged(int updateType) = 0;
	virtual void					PreDataUpdate(int updateType) = 0;
	virtual void					PostDataUpdate(int updateType) = 0;
	virtual void					__unkn() = 0;
	virtual bool					IsDormant() = 0; //9
 	virtual int						EntIndex() const = 0; //10
	virtual void					ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void*					GetDataTableBasePtr() = 0;
	virtual void					SetDestroyedOnRecreateEntities() = 0; //13
};