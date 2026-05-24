#pragma once
#include <tuple>
#include <type_traits>
#include "../interfaces/IBaseContext.h"
#include "../registries/registries.h"

/*
	ISystem - used to provide context for systems;

    Example usage:
    class EntityCacherSystem : public ISystem<EntityContext, SchemaContext>
*/

template<typename... Args> requires (std::derived_from<Args, IBaseContext> && ...)
class ISystem
{
public:
    ISystem() = default;
    virtual ~ISystem() = default;

protected:
    template<typename T> requires (std::is_same_v<T, Args> || ...)
    T& GetContext()
    {
        return core::registries::g_ContextRegistry.GetContext<T>();
    }

    template<typename T> requires (std::is_same_v<T, Args> || ...)
    const T& GetContext() const
    {
        return core::registries::g_ContextRegistry.GetContext<T>();
    }
};