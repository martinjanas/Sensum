#include "dynamic_module.h"

void DynamicModule::PrintAllInterfaces()
{
	auto interface_registry = *exporter.get_export("CreateInterface").add(0x3).abs().as<InterfaceReg**>();
	if (!interface_registry)
		return;

	for (auto& reg = interface_registry; reg != nullptr; reg = reg->m_pNext)
	{
		printf("name: %s\n", reg->m_pName);
	}
}

Exporter& DynamicModule::get_export(const std::string_view& func_name)
{
	return exporter.get_export(func_name);
}

PatternScanner& DynamicModule::scan(const char* signature, const char* msg)
{
	return pattern_scanner.scan(signature, msg);
}