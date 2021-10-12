#pragma once

template<typename FuncType>
__forceinline static FuncType CallVFunction(void* ppClass, int index)
{
	return (*static_cast<FuncType * *>(ppClass))[index];
}

template <std::size_t Index, typename ReturnType, typename... Args>
__forceinline ReturnType call_virtual(void* instance, Args... args)
{
	using Fn = ReturnType(__thiscall*)(void*, Args...);

	auto function = (*reinterpret_cast<Fn**>(instance))[Index];
	return function(instance, args...);
}