#pragma once

#include "../misc/IHandleEntity.hpp"

//class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class c_base_entity;
class IClientThinkable;
class IClientAlphaProperty;

enum SolidType_t;

class ICollideable
{
public:
	virtual IHandleEntity* GetEntityHandle() = 0;
	virtual const Vector& OBBMins() const = 0;
	virtual const Vector& OBBMaxs() const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual c_base_entity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
	//virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};