#pragma once

class IBaseContext
{
public:
	virtual ~IBaseContext() = default;

public:
	virtual void ConstructContext() { }
};
