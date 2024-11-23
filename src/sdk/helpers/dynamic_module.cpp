#include "dynamic_module.h"

void DynamicModule::dump_all_interfaces()
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

PatternScanner& DynamicModule::scan(const std::string_view& signature, const std::string_view& msg)
{
	return pattern_scanner.scan(signature.data(), msg.data());
}

void DynamicModule::scan_and_cache_sig(const std::string_view& sig, const std::string_view& sig_name, const uintptr_t& offset, bool abs)
{
	const auto& hash = fnv::hash_runtime(sig_name.data());

	if (abs)
		sig_addr_cacher.cache_sig(hash, scan(sig, sig_name).add(offset).abs());
	else sig_addr_cacher.cache_sig(hash, scan(sig, sig_name));
}

PatternScanner& DynamicModule::get_sig_addr(const fnv::hash& hash, const char* func_name)
{
	return sig_addr_cacher.get_addr(hash, func_name);
}

