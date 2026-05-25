#pragma once
#include <tuple>
#include <type_traits>
#include "../contexts/BaseContext.h"
#include "../core.h"

/*
	ISystem - used to provide context for systems;

    Example usage:
    class EntityCacherSystem : public ISystem<EntityContext, SchemaContext>
*/

template<typename... Args> requires (std::derived_from<Args, BaseContext> && ...)
class BaseSystem
{
public:
    BaseSystem() = default;
    virtual ~BaseSystem() = default;

    virtual void OnCreate() = 0;
    virtual void OnUpdate() = 0;

protected:
    template<typename T> requires (std::is_same_v<T, Args> || ...)
    T& GetContext()
    {
        return core::contexts().GetContext<T>();
    }

    template<typename T> requires (std::is_same_v<T, Args> || ...)
    const T& GetContext() const
    {
        return core::contexts().GetContext<T>();
    }
};