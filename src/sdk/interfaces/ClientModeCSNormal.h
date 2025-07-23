#pragma once
#include "impl/Interface.h"

class CClientModeCSNormal : public Interface
{
public:
    CClientModeCSNormal(void* obj, const char* name) : Interface(obj, name) { }
};