#pragma once

template<typename FuncType>
__forceinline static FuncType CallVFunction(void* ppClass, int index)
{
	return (*static_cast<FuncType * *>(ppClass))[index];
}

template <std::size_t index, typename return_type, typename... Args>
__forceinline return_type call_virtual(void* instance, Args... args)
{
	using fn = return_type(__thiscall*)(void*, Args...);

	auto function = (*reinterpret_cast<fn**>(instance))[index];
	return function(instance, args...);
}