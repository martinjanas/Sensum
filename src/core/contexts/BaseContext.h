#pragma once

class BaseContext
{
public:
	virtual ~BaseContext() = default;

public:
	virtual void ConstructContext() { }
};
