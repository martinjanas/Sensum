#pragma once
#include <typeindex>
#include <memory>
#include <unordered_map>
#include <cassert>
#include "../contexts/BaseContext.h"

class ContextRegistry
{
public:
    template<typename T> requires std::derived_from<T, BaseContext>
    void Register()
    {
        auto ptr = std::make_unique<T>();
        m_Contexts.emplace(std::type_index(typeid(T)), std::move(ptr));
    }

    void ConstructContexts()
    {
        for (const auto& [type_index, ctx] : m_Contexts)
			ctx->ConstructContext();
	}

    template<typename T>
    T& GetContext()
    {
        auto it = m_Contexts.find(std::type_index(typeid(T)));

        assert(it != m_Contexts.end());

        return *static_cast<T*>(it->second.get());
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<BaseContext>> m_Contexts;
};



