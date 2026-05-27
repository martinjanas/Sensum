#pragma once

//BaseSubContext - used inside contexts...
class BaseSubContext
{
public:
	virtual ~BaseSubContext() = default;

public:
	virtual void ConstructSubContext() {}
};

